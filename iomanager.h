#pragma once
#include <stdint.h>

/*
 * Endereços dos registradores GPIO
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

static const uint32_t segments[16] = {
    0b1011111, // 0
    0b0000110, // 1
    0b0111011, // 2
    0b0101111, // 3
    0b1100110, // 4
    0b1101101, // 5
    0b1111101, // 6
    0b0000111, // 7
    0b1111111, // 8
    0b1101111, // 9
    0b1110111, // A
    0b1111100, // B
    0b1011001, // C
    0b0111110, // D
    0b1111001, // E
    0b1110001  // F
};

enum led {green1 = 4, yellow = 5, orange = 6, green2 = 7};
typedef enum led led_t;

void setupLeds();
void setupDisplay();

void lightUpLed(led_t ledColor);

void setDisplay(int value);
void clearDisplay();

void clearLeds();