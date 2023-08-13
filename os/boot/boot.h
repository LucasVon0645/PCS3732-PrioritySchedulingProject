#pragma once
#include "../queue/multiqueue.h"

extern multiqueue_t multi_queue;
extern tcb_t* os_tcb;

// Pontos de entrada dos threads
int os_thread(void);
int user_thread(void);


// Ponto de saída das threads
int halt();

void mfqs_scheduler();

tcb_t* create_tcb(uint32_t tid, uint32_t priority, uint32_t exc_slots, uint32_t age, uint32_t entry_point);