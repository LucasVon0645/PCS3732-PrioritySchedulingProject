#include "timer.h"

void init_timer(void) {
    bit_clr(INTMOD, 11);   // configura timer0 como IRQ
    bit_clr(INTMSK, 11);   // habilita interrupção do timer 0
    bit_clr(INTMSK, 21);   // habilita interrupções globais 

    TDATA1 = TIME_LIMIT;
    TCNT0 = TDATA1;

    TMOD = (TMOD & (~0b111 << 3) | (0b001 << 3));  // ativa o timer 0
}

void enable_timer(void) {
    TMOD = (TMOD & (~0b111) | (0b001));  // ativa o timer 0
}

void stop_timer(void) {
    TMOD = TMOD & (~0b111);  // desativa o timer 0
}