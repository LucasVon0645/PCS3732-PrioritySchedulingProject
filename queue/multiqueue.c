#include <stdint.h>
#include "multiqueue.h"

int highest_nonempty_queue_head(MultilevelQueue* multi_queue) {
    for(int i = 0; i < NUM_OF_QUEUES; i++) {
        if (!isEmpty(multi_queue->queues[i])) return front(multi_queue->queues[i]);
    }
    return 0;
}

int enqueue_by_priority(int tid, int priority, MultilevelQueue* multi_queue) {
    Queue* queue_to_use = multi_queue->queues[priority];
    if (isFull(queue_to_use)) {
        return 0;
    }
    enqueue(multi_queue->queues[priority], tid);
    return 1;
}

int dequeue_by_priority(int priority, MultilevelQueue* multi_queue) {
    Queue* queue_to_use = multi_queue->queues[priority];
    if (isEmpty(queue_to_use)) {
        return 0;
    }
    return dequeue(multi_queue->queues[priority]);
}

int queue_max_quanta(int priority, MultilevelQueue* multi_queue) {
    return multi_queue->queue_quanta[priority];
}