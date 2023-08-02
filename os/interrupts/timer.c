#include "timer.h"

void init_timer(void) {
    bit_clr(INTMOD, 10);     // configura timer0 como IRQ
    bit_clr(INTMSK, 10);     // habilita interrupção do timer 0
    bit_clr(INTMSK, 21);     // habilita interrupções globais 

    TDATA0 = TIME_LIMIT;          // valor para 0.1s com clock de 50 MHz
    TCNT0 = TDATA0;

    TMOD = TMOD & (~0b111);  // desativa o timer 0
}

void enable_timer(void) {
    TMOD = (TMOD & (~0b111) | (0b001));  // ativa o timer 0
}

void stop_timer(void) {
    TMOD = TMOD & (~0b111);  // desativa o timer 0
}