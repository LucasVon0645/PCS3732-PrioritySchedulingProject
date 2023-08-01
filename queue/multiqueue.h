#include <stdint.h>
#include "queue.h"

#define NUM_OF_QUEUES 3

typedef struct {
    Queue* queues[NUM_OF_QUEUES];
    int queue_quanta[NUM_OF_QUEUES];
} MultilevelQueue;

int highest_nonempty_queue_head(MultilevelQueue* multi_queue);
int enqueue_by_priority(int tid, int priority, MultilevelQueue* multi_queue);
int dequeue_by_priority(int priority, MultilevelQueue* multi_queue);
int queue_max_quanta(int priority, MultilevelQueue* multi_queue);