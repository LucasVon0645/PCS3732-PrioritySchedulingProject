#pragma once
#include "../queue/multiqueue.h"

extern multiqueue_t multi_queue;

// Pontos de entrada dos threads
int main(void);
int main2(void);

// Ponto de saída das threads
int halt();

void mfqs_scheduler();

tcb_t* create_tcb(uint32_t tid, uint32_t priority, uint32_t exc_slots, uint32_t age, uint32_t entry_point);