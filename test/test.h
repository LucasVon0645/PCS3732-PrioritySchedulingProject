#pragma once
#include "../os/scheduler/scheduler.h"

void queues_test_config();

void print_multi_queue(multiqueue_t* multiqueue);
void print_queue(queue_t* queue);
void print_tcb(tcb_t* tcb);

tcb_t* create_tcb(uint32_t tid, uint32_t priority, uint32_t exc_slots, uint32_t age);