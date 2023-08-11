#pragma once
#include "queue.h"

#define NUM_OF_QUEUES 3

typedef struct {
    queue_t* queues[NUM_OF_QUEUES];
    tcb_t* next_thread;
} multiqueue_t;

void upgrade_thread(tcb_t* thread, multiqueue_t* multi_queue);
void downgrade_thread(tcb_t* thread, multiqueue_t* multi_queue);
void age_all_threads(multiqueue_t* multi_queue);
void update_next_thread(multiqueue_t* multi_queue);
