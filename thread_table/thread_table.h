#include <stdint.h>

#define MAX_THREAD_NUM 10

// Estrutura do Thread Control Block (TCB).
typedef struct {
    uint32_t regs[17];   // contexto (17 registradores)
    uint32_t tid;        // identificador do processo
    uint32_t priority;   // prioridade atual do processo
    uint32_t exc_slots;  // numero de execucoes
} tcb;

 
// Estrutura da tabela de TCBs
typedef struct {
    tcb* threads[MAX_THREAD_NUM];
} thread_table;
