#pragma once
#include <stdint.h>
#include "../queue/multiqueue.h"

multiqueue_t multi_queue;
volatile tcb_t *current_tcb;
volatile uint32_t tid;

int mfqs_update_threads();
void mfqs_scheduler();
int get_current_priority();