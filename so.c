#include <stdint.h>

extern curr_tcb;

void halt(void) {
   software_interrupt(curr_tcb->tid, 0);
}

void yield(void) {
   software_interrupt(curr_tcb->tid, 1);
}

void __attribute__((naked))
software_interrupt(uint32_t tid, uint32_t type) {
    asm volatile ("mov r0, %[input_reg]\n\t" : : [input_reg] "r" (tid) : "r0");
    asm volatile ("mov r1, %[input_reg]\n\t" : : [input_reg] "r" (type) : "r1");
    asm volatile ("swi #0");
}

void __attribute__((interrupt ("SWI")))
software_interrupt_handler() {
    uint32_t tid, type;
    asm volatile ("mov %tid, r0\n\t" : "=r" (tid));
    asm volatile ("mov %type, r1\n\t" : "=r" (type));

    if (type == 0) {
        
    }
}