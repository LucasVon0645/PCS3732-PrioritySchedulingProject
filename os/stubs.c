#include <stdint.h>

extern uint32_t heap_start;

void *_sbrk(int incr) {
  static uint8_t *brk = (uint8_t*)&heap_start;
  uint8_t *prev;
  prev = brk;
  brk += incr;
  return prev;
}
