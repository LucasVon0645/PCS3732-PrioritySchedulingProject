#include "multiqueue.h"

void upgrade_thread(tcb_t* thread, multiqueue_t* multi_queue) {
    if (!(thread->priority)) return;

    queue_t* current_queue = multi_queue->queues[thread->priority];
    dequeue_by_tid(current_queue, thread->tid);

    queue_t* next_queue = multi_queue->queues[++thread->priority];
    thread->age = 0;
    thread->exc_slots = next_queue->quanta_limit;
    enqueue(next_queue, thread);
};

void downgrade_thread(tcb_t* thread, multiqueue_t* multi_queue) {
    if (thread->priority == NUM_OF_QUEUES - 1) return;

    queue_t* current_queue = multi_queue->queues[thread->priority];
    dequeue_by_tid(current_queue, thread->tid);

    queue_t* next_queue = multi_queue->queues[--thread->priority];
    thread->age = 0;
    thread->exc_slots = next_queue->quanta_limit;
    enqueue(next_queue, thread);
};

void age_all_threads(multiqueue_t* multi_queue) {
    for(int i = 1; i < NUM_OF_QUEUES; i++) {
        queue_t* current_queue = multi_queue->queues[i];
        node_t* current_node = current_queue->head;
        
        if (current_node) {
            node_t* tail_node = current_node->previous_node;
            int age_limit = current_queue->age_limit;

            while(current_node != tail_node) {
                tcb_t* current_tcb = current_node->tcb;

                if (++current_tcb->age > age_limit) 
                    upgrade_thread(current_tcb, multi_queue);

                current_node = current_node->next_node;
            }
        }
    }
}

void update_next_thread(multiqueue_t* multi_queue) {
    for(int i = 0; i < NUM_OF_QUEUES; i++) {
        if (multi_queue->queues[i]->head) {
            multi_queue->next_thread = multi_queue->queues[i]->head->tcb;
            return;
        }
    }
    multi_queue->next_thread = NULL;
}
