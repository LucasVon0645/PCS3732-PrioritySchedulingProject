#include <stdint.h>
#include "scheduler.h"


// Mock de threads
tcb tcb_array[3] = 
{
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr1,                              // sp
            0,                                       // lr inicial
            (uint32_t)main,                                    // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        0,
        1,
        5
    },
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr2,                              // sp
            0,                                       // lr inicial
            (uint32_t)main2,                                   // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        1,
        0,
        2
    },
    {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr3,                              // sp
            0,                                       // lr inicial
            (uint32_t)main3,                                   // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        2,
        0,
        2
    }
};

volatile int tid = 0;
volatile tcb *current_tcb = &tcb_array[0];
Queue queue0 = {{1, 2, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 1};
Queue queue1 = {{0}, 0, 0};
Queue queue2 = {{0}, -1, -1};
MultilevelQueue multi_queue = {{&queue0, &queue1, &queue2}, {2, 5, 10}};


void ageAllThreads() {}

void update_executed_thread(volatile tcb* cur_thread) {
   int tid = cur_thread->tid;
   int priority = cur_thread->priority;
   int remaining_quanta = --cur_thread->exc_slots;

    if (remaining_quanta == 0) {
        int new_priority = priority;

        if (priority < NUM_OF_QUEUES - 1) {
            new_priority = priority + 1;
            cur_thread->priority = new_priority;
        }

        cur_thread->exc_slots = queue_max_quanta(new_priority, &multi_queue);
        dequeue_by_priority(priority, &multi_queue);
        enqueue_by_priority(tid, new_priority, &multi_queue);
    }
}

void scheduler_mfqs(void) {
   update_executed_thread(current_tcb);
   int next_tid = highest_nonempty_queue_head(&multi_queue);
   tid = next_tid;
   current_tcb = &tcb_array[next_tid];
}