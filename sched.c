
#include <stdint.h>
#include "queue.h"

// Definidos pelo linker
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];

// Estrutura do Thread Control Block (TCB).
typedef struct {
   uint32_t pid;        // identificador do processo
   uint32_t regs[17];   // contexto (17 registradores)
   uint32_t priority;   // prioridade atual do processo
   uint32_t exc_slots;  // numero de execucoes
} tcb_t;


// Pontos de entrada dos threads
int main(void);
int main2(void);

// Mock de threads
tcb_t tcb_array[3] = 
{
    {
        1,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr1,                    // sp
            0,                                       // lr inicial
            (uint32_t)main,                          // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    },
    {
        2,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr2,                    // sp
            0,                                       // lr inicial
            (uint32_t)main2,                         // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    },
    {
        3,
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr2,                    // sp
            0,                                       // lr inicial
            (uint32_t)main2,                         // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        5
    }
};

volatile void *curr_tcb = &tcb_array[0];  // tcb do thread atual
volatile int tid = 0;

Queue queue0 = {{0}, -1,-1};
Queue queue1 = {{0}, -1, -1};
Queue queue2 = {{0}, -1, -1};
Queue* multilevel_queue[3] = {&queue0, &queue1, &queue2};


void ageAllThreads() {}

int thread_scheduler() {
   Queue* next_nonempty_queue;
   for(int i = 0; i < NUM_OF_QUEUES; i++) {
      Queue* queue_i = multilevel_queue[i];
      next_nonempty_queue = !isEmpty(queue_i) ? queue_i : next_nonempty_queue;
   }
   return dequeue(next_nonempty_queue);
}

void update_executed_thread(volatile tcb_t* cur_thread) {
   int pid = cur_thread->pid;
   int priority = cur_thread->priority;
   int remaining_quantums = cur_thread->exc_slots - 1;
   Queue* current_queue = multilevel_queue[priority];

   if (remaining_quantums == 0 && priority < NUM_OF_QUEUES - 1) {
      Queue* next_queue = multilevel_queue[priority];
      dequeue(current_queue);
      enqueue(next_queue, pid);
   }
}

void schedule_mfqs(void) {
   update_executed_thread(curr_tcb);
   int next_tid = thread_scheduler();
   tid = next_tid;
   curr_tcb = &tcb_array[next_tid];
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