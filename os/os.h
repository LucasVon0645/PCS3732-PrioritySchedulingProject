#include "types.h"
#include "threads/threads.h"
#include "scheduler/scheduler.h"

extern tcb_t *current_tcb;
extern multiqueue_t multi_queue;

int _get_current_priority();
int _get_current_cpu_time();
int _fork();
