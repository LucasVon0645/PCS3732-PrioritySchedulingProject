.include "evlt7t.inc"

/**
 * Vetor de interrupções do ARM
 */
.section .reset, "ax"
.org 0
vetor:
   ldr pc, reset_addr         // reset
   bl panic                    // instrução não definida
   ldr pc, swi_addr           // interrupção de software (chamada do kernel)
   bl panic                    // abort de instrução
   bl panic                    // abort de dados
   nop
   ldr pc, irq_addr           // interrupção
   bl panic                    // interrupção rápida

panic:
   b panic

reset_addr: .word reset
swi_addr:   .word handle_swi
irq_addr:   .word handle_irq

.text
/*
 * Ponto de entrada após reset.
 */
reset:
   // Configura modo IRQ
   mov r0, #0b10010
   msr cpsr, r0
   ldr sp, =stack_irq

   // Configura modo SVR:
   mov r0, #0b10011
   orr r0, r0, #(1 << 7) // configura bit IRQ para '1' (desabilitado)
   msr cpsr, r0
   ldr sp, =stack_svr

   // Zera segmento .bss:
   mov r0, #0
   ldr r1, =inicio_bss
   ldr r2, =fim_bss
loop_zera:
   cmp r1, r2
   bge start
   str r0, [r1], #4
   b loop_zera

/*
 * Ponto de entrada: executa o primeiro thread (tid = 0).
 */
start:
   bl boot
   bl setupLeds
   bl setupDisplay
   bl setup_user_button_int
   bl init_timer
   b context_change

/*
 * Ponto de entrada do kernel.
 * Identifica a função solicitada e trata.
 */
 handle_swi:
   // desativar iterrupção
   push {r1}
   mrs r1, cpsr
   orr r1, r1, #(1 << 7) // configura bit IRQ para '1' (desabilitado)
   msr cpsr, r1
   pop {r1}

   cmp r0, #1          // função yield: troca de thread
   beq thread_switch_swi

   cmp r0, #2          // função getpid: retorna a identificação do thread atual
   beq handle_getid

   cmp r0, #5
   beq handle_halt

   push {r1-r3, lr}
   cmp r0, #3
   bleq get_current_priority // função get_current_priority: retorna prioridade do thread atual

   cmp r0, #4
   bleq get_current_cpu_time // função get_cpu_time: retorna tempo de cpu usado até o momento pela thread atual
   pop {r1-r3, lr}

   // outras funções do kernel vão aqui...
   movs pc, lr          // retorna

/* Tratamento das interrupções IRQ */
handle_irq:
  /*
   * Salva registradores e verifica causa da interrupção.
   */
  push {r0-r3, lr}
  ldr r1, =INTPND
  ldr r0, [r1]  // r0 contém INTPND
test_timer_interrupt:
  tst r0, #(1 << 11) // r0 possui valor apontado por INTPND
  beq test_button_interrupt // se a interrupção não é do timere1, verifica se é do user interrupt
  mov r0, #0 // passa yield = 0 como parâmetro de mfqs_update_threads
  b mfqs_update_threads_call

test_button_interrupt:
  tst r0, #1 // r0 possui valor apontado por INTPND
  beq exit_handle_irq // interrupções que não são do timer1 ou do user interrupt são ignoradas
  mov r0, #1 // passa yield = 1 como parâmetro de mfqs_update_threads e executa mfqs_update_threads_call abaixo

mfqs_update_threads_call:
  bl mfqs_update_threads // atualiza a thread com parâmetro r0 passado anteriormente
  cmp r0, #1 // verifica se o retorno de mfqs_update_threads é 1 (troca de contexto necessária nesse caso)
  beq thread_switch_irq // continua em exit_handle_irq caso contrário

exit_handle_irq: 
  bl recognize_all_interrupts
  pop {r0-r3, lr}
  subs pc, lr, #4 // retorno para ponto de execução da atual thread sem troca de contexto

/* Retorna a identificação do thread atual. */
handle_getid:
   ldr r0, =tid
   ldr r0, [r0]
   movs pc, lr

/* Finaliza a thread atual e realiza troca de contexto para a próxima thread */
handle_halt:
   bl finish_current_thread // atualização do multi_queue.next_thread e o aging também são feitos em finish_current_thread
   bl mfqs_scheduler // current_tcb é atualizado aqui
   b context_change
  

/* Salva o contexto do usuário no tcb, com escalonamento cooperativo */
thread_switch_swi:
   push {r0}
   ldr r0, =current_tcb
   ldr r0, [r0]
   stmib r0, {r1-r14}^          // registradores r1-r14 do usuário
   
   // salva endereço de retorno (lr)
   str lr, [r0, #60]

   // copia o spsr do usuário em r1 e salva no tcb
   mrs r1, spsr
   str r1, [r0, #64]
   
   // finalmente, salva o r0 original no tcb
   pop {r1}
   str r1, [r0]

   // escala o próximo processo 
   bl mfqs_scheduler

/* Troca de contextos com escalonamento preemptivo */
thread_switch_irq:
   pop {r0-r3, lr}
   push {r0}
   ldr r0, =current_tcb
   ldr r0, [r0]
   stmib r0, {r1-r14}^ // registradores r1-r14 do usuário
   
   // salva endereço de retorno (lr)
   sub r1, lr, #4
   str r1, [r0, #60]

   // copia o spsr do usuário em r1 e salva no tcb
   mrs r1, spsr
   str r1, [r0, #64]
   
   // finalmente, salva o r0 original no tcb
   pop {r1}
   str r1, [r0]

   // escala o próximo processo 
   bl mfqs_scheduler

/* Retorna no contexto de outro thread */
context_change:
   ldr r0, =current_tcb
   ldr r0, [r0]

   // restaura spsr do usuário.
   ldr r1, [r0, #64]
   msr spsr, r1

   // Restaura r1-r14 originais, lr de retorno e finalmente r0
   ldmib r0, {r1-r14}^
   ldr lr, [r0, #60]
   ldr r0, [r0]

   // reconhece todas interrupções, caso haja
   push {r0-r3, lr}
   bl recognize_all_interrupts
   pop {r0-r3, lr}

   // retorna para o thread, mudando o modo 
   movs pc, lr
   