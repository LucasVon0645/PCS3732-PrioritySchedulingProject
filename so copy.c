#include <stdint.h>
#include "sched.h"

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


void  __attribute__((interrupt("FIQ")))
fiq_handler() {
    // thread 1
    asm volatile ("push {r0-r3, lr} \n\t"
    : : : "r0", "r1", "r2", "r3", "lr", "memory");

    stop_timer();
    int timer_interrupt = check_interruption_origin();

    if (timer_interrupt) {
        int schedule_new_thread = update_executed_thread(current_tcb);
        if (schedule_new_thread) {
            // thread 1
            asm volatile ("pop {r0-r3, lr} \n\t"
            : : : "r0", "r1", "r2", "r3", "lr", "memory");
            save_context();

            mfqs_scheduler();

            // thread 2
            change_context();
            asm volatile ("push {r0-r3, lr} \n\t"
            : : : "r0", "r1", "r2", "r3", "lr", "memory");
            recognize_all_interrupts();
            continue_timer();
            asm volatile ("pop {r0-r3, lr} \n\t"
            : : : "r0", "r1", "r2", "r3", "lr", "memory");
            asm volatile ("add lr, lr, #4 \n\t" : : : "lr" )
            return;
        }
    }

    recognize_all_interrupts();
    continue_timer();

    asm volatile ("pop {r0-r3, lr} \n\t"
    : : : "r0", "r1", "r2", "r3", "lr", "memory");

    return;
}
