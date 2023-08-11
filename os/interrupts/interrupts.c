#include "interrupts.h"

void recognize_all_interrupts() {
    INTPND = INTPND;
}

void setup_user_button_int() {
    bit_clr(INTMOD, 0);     // configura interrupção do user button como IRQ
    bit_clr(INTMSK, 0);     // habilita interrupção do user button
    bit_clr(INTMSK, 21);     // habilita interrupções globais

    bit_clr(IOPMOD, 8); // garante que a GPIO do user button (GPIO 8) é uma entrada
    IOPCON |= 0b11001; //  Habilita interrupção do user button para borda de subida, ativo alto
}