.include "evlt7t.inc"

.data
.set TEMPO, 50000000     // valor de recarga para 0.5s em 50 MHz

.text
.global init_timer1
init_timer1:
    // configura interrupção 11 (timer 1) como IRQ
    ldr r2, =INTMOD
    ldr r1, [r2]
    bic r1, r1, #(1 << 11)      // configura como IRQ
    str r1, [r2]

    // habilita a interrupção 21 (global) e habilita interrupção do timer1
    ldr r2, =INTMSK
    ldr r1, [r2]
    bic r1, r1, #(1 << 11) // bic ativa (0), orr desativa (1)
    bic r1, r1, #(1 << 21)
    str r1, [r2]

    // configura valor de recarga de 1s (período)
    ldr r2, =TDATA1
    ldr r1, =TEMPO
    str r1, [r2]

    // habilita o timer 1
    ldr r2, =TMOD
    ldr r1, [r2]
    bic r1, r1, #(0b111 << 3)    // reinicia modo do timer 1
    orr r1, r1, #(0b001 << 3)    // habilita o timer
    str r1, [r2]

    bx lr                       // retorna da subrotina

.global enable_timer1_int
enable_timer1_int:
   // liga timer1
   ldr r2, =TMOD
   ldr r1, [r2]
   bic r1, r1, #(0b111 << 3)
   orr r1, r1, #(0b001 << 3)    // habilita o timer
   str r1, [r2]
   bx lr

.global disable_timer1_int
disable_timer1_int:
   // desligar o timer 1
   ldr r2, =TMOD
   ldr r1, [r2]
   bic r1, r1, #(0b111 << 3)    // reinicia modo do timer 1
   str r1, [r2]
   bx lr
