#include <stdint.h>
#include "queue.h"

#define NUM_OF_QUEUES 3

typedef struct {
    Queue* queues[NUM_OF_QUEUES];
} MultilevelQueue;

int dequeue_highest_nonempty_queue(MultilevelQueue* multi_queue);
int enqueue_by_priority(int tid, int priority, MultilevelQueue* multi_queue);
int dequeue_by_priority(int priority, MultilevelQueue* multi_queue);