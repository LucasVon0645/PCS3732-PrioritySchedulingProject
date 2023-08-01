#include "iomanager.h"

void setupLeds() {
    int32_t mask = 0b1111 << 4;
    IOPMOD |= mask; // configura leds como saída
    IOPDATA &= ~(mask); // desliga todos os leds
}

void setupDisplay() {
    uint32_t mask = 0b1111111 << 10;
    IOPMOD |= mask; // configura display como saída
    IOPDATA &= ~(mask); // desliga todos os segmentos do display
}

void lightUpLed(led_t ledColor) {
    uint32_t mask = 0b1111 << 4;
    IOPDATA &= ~(mask);
    mask = 0b1 << ledColor;
    IOPDATA |= mask;
}

void setDisplay(int value) {
    // Verifique se o valor está dentro do intervalo de 0 a 15
    if (value >= 0 && value <= 15) {
        uint32_t newCode = segments[value] << 10;
        uint32_t mask = 0b1111111 << 10;

        if ((IOPDATA & mask) != newCode) {
            IOPDATA &= ~(mask);
            IOPDATA |= newCode;
        }
    }
}

void clearDisplay() {
    uint32_t mask = 0b1111111 << 10;
    IOPDATA &= ~(mask);
}

void clearLeds() {
    uint32_t mask = 0b1111 << 4;
    IOPDATA &= ~(mask);
}