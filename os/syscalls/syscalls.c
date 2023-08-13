#include "syscalls.h"

/**
 * Chama o kernel com swi, a função "yield" (r0 = 1).
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void __attribute__((naked)) yield(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #1 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}

/**
 * Retorna o thread-id do thread atual.
 */
int __attribute__((naked)) get_pid(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #2 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}

int __attribute__((naked)) get_priority(void) {
      asm volatile("push {lr}  \n\t"
                   "mov r0, #3 \n\t"
                   "swi #0     \n\t"
                   "pop {pc}");
}

int __attribute__((naked)) get_cpu_time(void) {
      asm volatile("push {lr}  \n\t"
                   "mov r0, #4 \n\t"
                   "swi #0     \n\t"
                   "pop {pc}");
}

void __attribute__((naked)) halt(void) {
      asm volatile("mov r0, #5 \n\t"
                   "swi #0     \n\t");
}

void __attribute__((naked)) fork(void) {
      asm volatile("push {lr}  \n\t"
                   "mov r0, #6 \n\t"
                   "swi #0     \n\t"
                   "pop {pc}");
}