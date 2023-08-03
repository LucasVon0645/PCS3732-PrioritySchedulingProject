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

void ageAllThreads();

int update_executed_thread();

void scheduler_mfqs(void);