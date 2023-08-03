#include "queue.h"

#define NUM_OF_QUEUES 3

typedef struct {
    queue_t* queues[NUM_OF_QUEUES];
} multiqueue_t;

tcb_t* highest_nonempty_queue_head(multiqueue_t* multi_queue);
void upgrade_thread(tcb_t* thread, multiqueue_t* multi_queue);
void downgrade_thread(tcb_t* thread, multiqueue_t* multi_queue);
void age_all_threads(multiqueue_t* multi_queue);