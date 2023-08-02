.set INTPND, 0x03ff4004

/*
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

   bl init_timer

   // Zera segmento .bss:
   mov r0, #0
   ldr r1, =inicio_bss
   ldr r2, =fim_bss

loop_bss:
   cmp r1, r2
   bge start
   str r0, [r1], #4
   b loop_bss

/*
 * Ponto de entrada: executa o primeiro thread (tid = 0).
 */
start:
   mov r0, #0              // tid = 0
   ldr r1, =tid
   str r0, [r1]
   ldr r0, =tcb_array            // current_tcb = &tcb[0]
   ldr r1, =current_tcb
   str r0, [r1]
   b restore_context

/*
 * Ponto de entrada do kernel.
 * Identifica a função solicitada e trata.
 */
handle_swi:
   // desativar iterrupção
   @ cmp r0, #1          // função yield: troca de thread
   @ beq thread_switch_swi

   cmp r0, #2          // função getpid: retorna a identificação do thread atual
   beq get_tid

   // outras funções do kernel vão aqui...
   movs pc, lr          // retorna


handle_irq:
  /*
   * Salva registradores e verifica causa da interrupção.
   */
  push {r0, r1, r2, lr}
  ldr r1, =INTPND
  ldr r0, [r1]  // r0 contém INTPND
  tst r0, #(1 << 10)
  bne timer0_irq // tratamento para interrupção do timer1 -> mudança de contexto
  bl recognize_all_interrupts // ignora interrupções que não sejam do timer1
  pop {r0, r1, r2, lr}
  subs pc, lr, #4 // retorno para ponto de execução da atual thread sem troca de contexto

timer0_irq:
   bl stop_timer
   pop {r0, r1, r2, lr}
   b save_context_irq

/* Troca de contextos com escalonamento preemptivo */
save_context_irq:
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
   bl scheduler_mfqs

/* Retorna no contexto de outro thread */
restore_context:
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
   push {r1, r2, lr}
   bl recognize_all_interrupts
   // habilita timer1
   bl enable_timer
   pop {r1, r2, lr}

   // retorna para o thread, mudando o modo 
   movs pc, lr
   
/* Retorna a identificação do thread atual. */
get_tid:
   ldr r0, =tid
   ldr r0, [r0]
   movs pc, lr
