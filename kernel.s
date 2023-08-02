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
swi_addr:   .word trata_swi
irq_addr:   .word trata_irq

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

   // Configura modo USR:
   @ mov r0, #0b10000
   @ bic r0, r0, #(1 << 7) // configura bit IRQ para '0' (habilitado)
   @ msr cpsr, r0
   @ ldr sp, =stack_usr

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
   ldr r0, =tcb_array            // current_tcb = &tcb[0]
   ldr r1, =current_tcb
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
   bl schedule_mfqs

/* Troca de contextos com escalonamento preemptivo */
save_context:
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
   bl schedule_mfqs

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
   push {r1, r2, lr}
   bl reconhece_irq
   // habilita timer1
   bl enable_timer1_int
   pop {r1, r2, lr}

   // retorna para o thread, mudando o modo 
   movs pc, lr
   