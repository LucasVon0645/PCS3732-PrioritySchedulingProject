#include "utils.h"

void* aligned_malloc(size_t required_bytes, size_t alignment) {
    void* p1;
    void** p2;
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(required_bytes + offset)) == NULL) {
      return NULL;
    }
    p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void *p) {
    free(((void**)p)[-1]);
}

// p/t &(new_tcb->stack[0])
// p/t &(current_tcb->stack[2])
// p/t &(new_tcb->stack[4094])


// p/t &(current_tcb->stack[4])

// p/x *(multi_queue.queues[0]->head->tcb)
// display /x (multi_queue.queues[0]->head->tcb->tid)
// display /x (multi_queue.queues[0]->head->next_node->tcb->tid)
// display /x (multi_queue.queues[0]->head->previous_node->tcb->tid)
// display /x (multi_queue.queues[1]->head->tcb->tid)
// display /x (multi_queue.queues[1]->head->next_node->tcb->tid)
// display /x (multi_queue.queues[1]->head->previous_node->tcb->tid)
// display /x (multi_queue.queues[2]->head->tcb->tid)
// display /x (multi_queue.queues[2]->head->next_node->tcb->tid)
// display /x (multi_queue.queues[2]->head->previous_node->tcb->tid)
// display /x (multi_queue.queues[0]->head)
// display /x (multi_queue.queues[1]->head)
// display /x (multi_queue.queues[2]->head)
// p/x (current_tcb->tid)