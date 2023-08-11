#pragma once
#include <stdint.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 10


typedef struct {
    uint32_t regs[17];   // contexto (17 registradores)
    uint32_t tid;        // identificador da thread
    uint32_t priority;   // prioridade atual da thread
    uint32_t exc_slots;  // numero de execucoes restantes
    uint32_t age;        // numero de execucoes restantes
    uint32_t cpu_time;   // quantidade total de execucoes de slots de tempo da thread
    uint8_t stack[4096]; // stack
} tcb_t;


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