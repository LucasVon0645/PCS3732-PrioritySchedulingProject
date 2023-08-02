#include <stdint.h>
#include "sched.h"
#include "timer.h"

extern void save_context();
extern void change_context();


void check_interrupt_origin() {
    return bit_is_set(INTPND, 10);
}

void recognize_all_interrupts() {
    INTPND = INTPND;
}


void  __attribute__((interrupt("FIQ")))
fiq_handler() {
    // thread 1
    asm volatile ("push {lr} \n\t"
    : : : "r0", "r1", "r2", "r3", "lr", "memory");
    asm volatile ("push {r0-r3} \n\t"
    : : : "r0", "r1", "r2", "r3", "lr", "memory");

    stop_timer();
    int timer_interrupt = check_interrupt_origin();

    if (timer_interrupt) {
        int schedule_new_thread = update_executed_thread(current_tcb);
        if (schedule_new_thread) {
            // ------ Salve contexto da thread A  -------
            asm volatile ("pop {r0-r3} \n\t"
            : : : "r0", "r1", "r2", "r3", "lr", "memory");
            save_context();

            // ------ Pega a proxima thread B  -------
            mfqs_scheduler();

            // ------ Recupera contexto da thread B  -------
            change_context();

            // ------ Faz house cleaning e retorna  -------
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
