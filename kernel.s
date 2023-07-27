.include "evlt7t.inc"
.include "timer.s"

/**
 * Vetor de interrupções do ARM
 */
.section .reset, "ax"
.org 0
vetor:
   ldr pc, reset_addr         // reset
   b panic                    // instrução não definida
   ldr pc, swi_addr           // interrupção de software (chamada do kernel)
   b panic                    // abort de instrução
   b panic                    // abort de dados
   nop
   ldr pc, irq_addr           // interrupção
   b panic                    // interrupção rápida

panic:
   b panic

reset_addr: .word reset
swi_addr:   .word trata_swi
irq_addr:   .word trata_irq

.data
.set TEMPO, 499999990    // valor de recarga para 1s em 50 MHz

.text
/*
 * Ponto de entrada após reset.
 */
reset:
   // Configura sp do modo IRQ
   mov r0, #0b10010
   msr cpsr, r0
   ldr sp, =stack_irq

   // Configura sp do modo SVR:
   mov r0, #0b10011
   msr cpsr, r0
   ldr sp, =stack_svr

   bl init_timer1

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
   mov r0, #0              // tid = 0
   ldr r1, =tid
   str r0, [r1]
   ldr r0, =tcb_array            // curr_tcb = &tcb[0]
   ldr r1, =curr_tcb
   str r0, [r1]
   bl setupLeds
   bl setupDisplay
   b context_change

/*
 * Ponto de entrada do kernel.
 * Identifica a função solicitada e trata.
 */
trata_swi:
   // desativar iterrupção
   cmp r0, #1          // função yield: troca de thread
   beq thread_switch_swi

   cmp r0, #2          // função getpid: retorna a identificação do thread atual
   beq getid

   // outras funções do kernel vão aqui...
   movs pc, lr          // retorna

trata_irq:
  /*
   * Salva registradores e verifica causa da interrupção.
   */
  push {r0, r1, r2, lr}
  ldr r1, =INTPND
  ldr r0, [r1]  // r0 contém INTPND
  tst r0, #(1 << 11)
  bne timer1_irq // tratamento para interrupção do timer1 -> mudança de contexto
  bl reconhece_irq // ignora interrupções que não sejam do timer1
  pop {r0, r1, r2, lr}
  subs pc, lr, #4 // retorno para ponto de execução da atual thread sem troca de contexto
timer1_irq:
   bl disable_timer1_int
   bl clearDisplay
   bl clearLeds
   pop {r0, r1, r2, lr}
   b thread_switch_irq

reconhece_irq:
  /*
   * Fim do tratamento.
   * Reconhece todas as interrupções.
   */
  ldr r1, =INTPND
  ldr r0, [r1]
  str r0, [r1]
  bx lr

/* Retorna a identificação do thread atual. */
getid:
   ldr r0, =tid
   ldr r0, [r0]
   movs pc, lr

/* Salva o contexto do usuário no tcb, com escalonamento cooperativo */
thread_switch_swi:
   push {r0}
   ldr r0, =curr_tcb
   ldr r0, [r0]
   stmib r0, {r1-r14}^          // registradores r1-r14 do usuário
   sub r1, lr, #4
   // salva endereço de retorno (lr) -> pc do user
   str r1, [r0, #60]

   // copia o spsr do usuário em r1 e salva no tcb
   mrs r1, spsr
   str r1, [r0, #64]
   
   // finalmente, salva o r0 original no tcb
   pop {r1}
   str r1, [r0]

   // escala o próximo processo 
   bl schedule_mfqs

/* Troca de contextos com escalonamento preemptivo */
thread_switch_irq:
   push {r0}
   ldr r0, =curr_tcb
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
   bl schedule_mfqs

/* Retorna no contexto de outro thread */
context_change:
   ldr r0, =curr_tcb
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
   bl reconhece_irq
   // habilita timer1
   bl enable_timer1_int
   pop {r1, r2, lr}

   // retorna para o thread, mudando o modo 
   movs pc, lr