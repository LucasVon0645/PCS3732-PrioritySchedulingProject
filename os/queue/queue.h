#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <../types.h>

#define MAX_QUEUE_SIZE 10


typedef struct node_t {
    tcb_t* tcb;
    struct node_t* next_node;
    struct node_t* previous_node;
} node_t;

 
// Estrutura da tabela de TCBs
typedef struct {
    node_t* head;
    uint32_t quanta_limit;
    uint32_t age_limit;
} queue_t;

void enqueue(queue_t* queue, tcb_t* new_tcb);
tcb_t* dequeue_by_tid(queue_t* queue, uint32_t tid);