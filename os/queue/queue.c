#include "queue.h"

void _dequeue(queue_t* queue, node_t* node) {
    node_t* previous_node = node->previous_node;
    node_t* next_node = node->next_node;

    // No elements in list
    if (!(queue->head)) return;

    // Single element in the list
    if (next_node == node) queue->head = NULL;

    // Only two elements in the list
    else if (next_node == previous_node) {
        next_node->next_node = NULL;
        next_node->previous_node = NULL;
        queue->head = next_node;
    }

    // Arbitrary number of elements in the list
    else {
        previous_node->next_node = next_node;
        next_node->previous_node = previous_node;
        if (queue->head == node) queue->head = next_node;
    }
}


tcb_t* dequeue_by_tid(queue_t* queue, int tid) {
    node_t* queue_head = queue->head;
    if (!(queue_head)) return 0;

    node_t* tail_node = queue_head->previous_node;
    node_t* current_node = current_node->next_node;

    while (current_node->tcb->tid != tid) {
        if (current_node == tail_node) return;
        current_node = current_node->next_node;
    }

    _dequeue(queue, current_node);
    return current_node->tcb;
}


void enqueue(queue_t* queue, tcb_t* new_tcb) {
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->tcb = new_tcb;

    if (queue->head == NULL) {
        queue->head = new_node;
        new_node->next_node = new_node;
        new_node->previous_node = new_node;
        return;
    }

    node_t* head_node = queue->head;
    node_t* tail_node = head_node->previous_node;

    head_node->previous_node = new_node;
    tail_node->previous_node = new_node;

    new_node->next_node = head_node;
    new_node->previous_node = tail_node;
}