#pragma once
#include <stdint.h>
#include "../queue/multiqueue.h"

extern multiqueue_t multi_queue;
extern tcb_t *current_tcb;
extern volatile uint32_t tid;
extern tcb_t* os_tcb;

int mfqs_update_threads(int yield);
void mfqs_scheduler();
void finish_current_thread();
int get_current_priority();
int get_current_cpu_time();