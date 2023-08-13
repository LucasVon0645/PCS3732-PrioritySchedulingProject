#include "types.h"
#include "threads/threads.h"
#include "scheduler/scheduler.h"

extern tcb_t *current_tcb;
multiqueue_t multi_queue;

int _get_current_priority() {
    int p = current_tcb->priority;
    return p;
}

int _get_current_cpu_time() {
    int cpu_time = current_tcb->cpu_time;
    return cpu_time;
}

int _fork() {
    tcb_t* new_tcb = copy_tcb(current_tcb);
    add_to_multiqueue(new_tcb, &multi_queue);
    return new_tcb->tid;
}
