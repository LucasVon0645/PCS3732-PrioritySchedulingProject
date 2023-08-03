#include <stdint.h>

extern uint32_t inicio_heap;

void *_sbrk(int incr) {
  static uint8_t *brk = (uint8_t*)&inicio_heap;
  uint8_t *prev;
  prev = brk;
  brk += incr;
  return prev;
}