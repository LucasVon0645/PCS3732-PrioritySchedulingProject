#include <stdint.h>
#include "queue/multiqueue.h"
//#include "thread_table/thread_table.h"

// Definidos pelo linker
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];
extern uint8_t stack_usr3[];

// Pontos de entrada dos threads
int main(void);
int main2(void);
int main3(void);

typedef struct {
    uint32_t regs[17];   // contexto (17 registradores)
    uint32_t tid;        // identificador da thread
    uint32_t priority;   // prioridade atual da thread
    uint32_t exc_slots;  // numero de execucoes restantes
} tcb;

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
        2
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
        5
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
        5
    }
};

volatile int tid = 0;
volatile tcb *current_tcb = &tcb_array[0];
Queue queue0 = {{1, 2, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 1};
Queue queue1 = {{0}, 0, 0};
Queue queue2 = {{0}, -1, -1};
MultilevelQueue multi_queue = {{&queue0, &queue1, &queue2}};


void ageAllThreads() {}

int thread_scheduler() {
    return highest_nonempty_queue_head(&multi_queue);
}

void update_executed_thread(volatile tcb* cur_thread) {
   int tid = cur_thread->tid;
   int priority = cur_thread->priority;
   int remaining_quantums = cur_thread->exc_slots--;

    if (remaining_quantums == 0 && priority < NUM_OF_QUEUES - 1) {
        cur_thread->priority = priority + 1;
        dequeue_by_priority(priority, &multi_queue);
        enqueue_by_priority(tid, priority + 1, &multi_queue);
    }


}

void schedule_mfqs(void) {
   update_executed_thread(current_tcb);
   int next_tid = thread_scheduler();
   tid = next_tid;
   current_tcb = &tcb_array[next_tid];
}

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
int __attribute__((naked)) getpid(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #2 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}