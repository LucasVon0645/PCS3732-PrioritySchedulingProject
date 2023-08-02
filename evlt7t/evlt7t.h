#pragma once

#include <stdint.h>

/*
 * Endereços dos registradores GPIO.
 */
#define IOPMOD  (*(volatile uint32_t*)0x03ff5000)
#define IOPCON  (*(volatile uint32_t*)0x03ff5004)
#define IOPDATA (*(volatile uint32_t*)0x03ff5008)

/*
 * Endereços dos registradores do controlador de interrupções.
 */
#define INTMOD (*(volatile uint32_t*)0x03ff4000)
#define INTPND (*(volatile uint32_t*)0x03ff4004)
#define INTMSK (*(volatile uint32_t*)0x03ff4008)

/*
 * Endereços dos registradores dos Timers.
 */
#define TMOD   (*(volatile uint32_t*)0x03ff6000)
#define TDATA0 (*(volatile uint32_t*)0x03ff6004)
#define TDATA1 (*(volatile uint32_t*)0x03ff6008)
#define TCNT0  (*(volatile uint32_t*)0x03ff600c)
#define TCNT1  (*(volatile uint32_t*)0x03ff6010)

/*
 * Contagem para 0.5 segundo em clock de 50 MHz.
 */
#define TIMER_DATA  25000000

/*
 * Endereços dos registradores da UART 0.
 */
#define ULCON0  (*(volatile uint32_t*)0x03ffd000)
#define UCON0   (*(volatile uint32_t*)0x03ffd004)
#define USTAT0  (*(volatile uint32_t*)0x03ffd008)
#define UTXBUF0 (*(volatile uint32_t*)0x03ffd00c)
#define URXBUF0 (*(volatile uint32_t*)0x03ffd010)
#define UBRDIV0 (*(volatile uint32_t*)0x03ffd014)

/*
 * Macros úteis
 */
#define bit_set(X, B) X |= (1 << B)
#define bit_clr(X, B) X &= ~(1 << B)
#define bit_inv(X, B) X ^= (1 << B)
#define bit_is_set(X, B) ( X & (1 << B) )