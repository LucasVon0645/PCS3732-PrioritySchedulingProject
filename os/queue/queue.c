#include "queue.h"

void _dequeue(queue_t* queue, node_t* node) {
    node_t* previous_node = node->previous_node;
    node_t* next_node = node->next_node;

    // Trtamento para quando não há elementos na fila
    if (!(queue->head)) return;

    // Remoção para quando há apenas um elemento na fila
    if (next_node == node) queue->head = NULL;

    // Tratamento para quando há dois elementos apenas na fila
    else if (next_node == previous_node) {
        next_node->next_node = next_node;
        next_node->previous_node = next_node;
        queue->head = next_node;
    }

    // Tratamento para demais casos
    else {
        previous_node->next_node = next_node;
        next_node->previous_node = previous_node;
        if (queue->head == node) queue->head = next_node;
    }
}


tcb_t* dequeue_by_tid(queue_t* queue, uint32_t tid) {
    node_t* queue_head = queue->head;
    if (!(queue_head)) return NULL;

    node_t* tail_node = queue_head->previous_node;
    node_t* current_node = queue_head;

    while (current_node->tcb->tid != tid) {
        if (current_node == tail_node) return NULL;
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
    tail_node->next_node = new_node;

    new_node->next_node = head_node;
    new_node->previous_node = tail_node;
}