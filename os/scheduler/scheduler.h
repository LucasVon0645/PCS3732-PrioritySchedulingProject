#pragma once
#include <stdint.h>
#include "../queue/multiqueue.h"

extern multiqueue_t multi_queue;
extern tcb_t *current_tcb;
extern volatile uint32_t tid;

int mfqs_update_threads();
void mfqs_scheduler();
int get_current_priority();