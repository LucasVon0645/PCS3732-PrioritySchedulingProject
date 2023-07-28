#include <stdint.h>
#include "multiqueue.h"
#include "thread_table.h"

// Definidos pelo linker
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];

// Pontos de entrada dos threads
int main(void);
int main2(void);

// Mock de threads
tcb tcb_array[3] = 
{
    {
        1,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            stack_usr1,                              // sp
            0,                                       // lr inicial
            main,                                    // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    },
    {
        2,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            stack_usr2,                              // sp
            0,                                       // lr inicial
            main2,                                   // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    },
    {
        3,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            stack_usr2,                              // sp
            0,                                       // lr inicial
            main2,                                   // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    }
};

volatile tcb *current_tcb = &tcb_array[0];
Queue queue0 = {{0}, -1,-1};
Queue queue1 = {{0}, -1, -1};
Queue queue2 = {{0}, -1, -1};
Queue* queue_array[3] = {&queue0, &queue1, &queue2};
MultilevelQueue* multi_queue = {queue_array};


void ageAllThreads() {}

int thread_scheduler() {
    Queue* next_nonempty_queue = dequeue_highest_nonempty_queue(&multi_queue);
    return dequeue(next_nonempty_queue);
}

void update_executed_thread(volatile tcb* cur_thread) {
   int tid = cur_thread->tid;
   int priority = cur_thread->priority;
   int remaining_quantums = cur_thread->exc_slots - 1;

    if (remaining_quantums == 0 && priority < NUM_OF_QUEUES - 1) {
        cur_thread->priority = priority + 1;
        dequeue_by_priority(priority, multi_queue);
        enqueue_by_priority(tid, priority + 1, multi_queue);
    }
}

void schedule_mfqs(void) {
   update_executed_thread(current_tcb);
   int next_tid = thread_scheduler();
   current_tcb = &tcb_array[next_tid];
}
