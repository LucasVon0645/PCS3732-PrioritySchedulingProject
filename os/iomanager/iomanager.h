#pragma once
#include <stdint.h>
#include "../../evlt7t/evlt7t.h"

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

enum led {green1 = 0, yellow = 1, orange = 2, green2 = 3};
typedef enum led led_t;

void setupLeds();
void setupDisplay();

void updateLed(led_t ledColor);
void updateDisplay(int value);

void showOsThreadOnDisplay();