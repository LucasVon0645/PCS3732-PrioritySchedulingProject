#include "timer.h"

void init_timer(void) {
    bit_set(INTMOD, 10);     // configura timer0 como FIQ
    bit_clr(INTMSK, 10);     // habilita interrupção do timer 0
    bit_clr(INTMSK, 21);     // habilita interrupções globais 

    TDATA0 = TEMPO;          // valor para 0.1s com clock de 50 MHz
    TCNT0 = TDATA0;

    TMOD = TMOD & (~0b111);  // desativa o timer 0
}

void continue_timer(void) {
    TMOD = (TMOD & (~0b111) | (0b001));  // ativa o timer 0
}

void stop_timer(void) {
    TMOD = TMOD & (~0b111);  // ativa o timer 0
}