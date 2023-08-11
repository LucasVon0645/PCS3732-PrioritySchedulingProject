#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "test.h"

int main()
{
    int change_context = 0;

    printf("Test begin!\n");
    tcb_t* tcb0 = create_tcb(0, 0, 2, 0);
    tcb_t* tcb1 = create_tcb(1, 0, 2, 0);
    tcb_t* tcb2 = create_tcb(2, 1, 5, 0);
    tcb_t* tcb3 = create_tcb(3, 1, 5, 0);
    tcb_t* tcb4 = create_tcb(4, 2, 10, 0);
    tcb_t* tcb5 = create_tcb(5, 2, 10, 0);

    queues_test_config();

    queue_t* queue0 = multi_queue.queues[0];
    queue_t* queue1 = multi_queue.queues[1];
    queue_t* queue2 = multi_queue.queues[2];

    enqueue(queue0, tcb0);
    enqueue(queue0, tcb1);
    enqueue(queue1, tcb2);
    enqueue(queue1, tcb3);
    enqueue(queue2, tcb4);
    enqueue(queue2, tcb5);
    update_next_thread(&multi_queue);
    mfqs_scheduler();

    for (int i = 0; i < 6; i++) {
        printf("\n Update threads %d \n", i);
        change_context = mfqs_update_threads();
        print_multi_queue(&multi_queue);
        if (change_context) {
            printf("\n MUDANCA DE CONTEXTO \n");
            printf("ANTES\n");
            print_tcb(current_tcb);
            mfqs_scheduler();
            printf("DEPOIS\n");
            print_tcb(current_tcb);
        }
    }

    return 0;
}


void queues_test_config() {
    queue_t *queue0 = (queue_t*)malloc(sizeof(queue_t));
    queue0->quanta_limit = 2;
    queue0->age_limit = 5;

    queue_t *queue1 = (queue_t*)malloc(sizeof(queue_t));
    queue1->quanta_limit = 5;
    queue1->age_limit = 4;

    queue_t *queue2 = (queue_t*)malloc(sizeof(queue_t));
    queue2->quanta_limit = 10;
    queue2->age_limit = 20;

    multi_queue.queues[0] = queue0;
    multi_queue.queues[1] = queue1;
    multi_queue.queues[2] = queue2;
}

void print_tcb(tcb_t* tcb) {
    printf(" Thread ID: %u\n", tcb->tid);
    printf(" Priority: %u\n", tcb->priority);
    printf(" Execution Slots: %u\n", tcb->exc_slots);
    printf(" Age: %u\n", tcb->age);
}

void print_queue(queue_t* queue) {
    printf("-------------------------------\n");
    if (queue->head == NULL) {
        printf("Empty queue\n");
        printf("-------------------------------\n");
        return;
    }

    int end = 0;

    node_t* head_node = queue->head;
    node_t* current = head_node;

    while (!end) {
        printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
        print_tcb(current->tcb);
        printf(" prev_tid: %d\n", current->previous_node->tcb->tid);
        printf(" next_tid: %d\n", current->next_node->tcb->tid);
        printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
        current = current->next_node;
        if (current == head_node) end = 1;
    }

    printf("-------------------------------\n");
}

void print_multi_queue(multiqueue_t* multiqueue) {
    printf("\n*************** multi queue start ***************\n");
    printf("\nMulti Queue:\n");
    for (int i = 0; i < 3; ++i) {
        if ( multiqueue->queues[i]->head) {
            int tail_tid = multiqueue->queues[i]->head->previous_node->tcb->tid;
            int head_tid = multiqueue->queues[i]->head->tcb->tid;
            printf("Queue %d:\n", i);
            printf("Head: tid = %d\n", head_tid);
            printf("Tail: tid = %d\n", tail_tid);
            printf("Age limit: = %d\n", multiqueue->queues[i]->age_limit);
            printf("Quanta Limit: = %d\n", multiqueue->queues[i]->quanta_limit);
            print_queue(multiqueue->queues[i]);
            printf("\n");
        }
       else {
        printf("Queue %d is empty\n", i);
       }
    }
    printf("\n*************** multi queue end ***************\n");
}

// Função para criar um elemento tcb_t
tcb_t* create_tcb(uint32_t tid, uint32_t priority, uint32_t exc_slots, uint32_t age) {
    tcb_t* new_tcb = (tcb_t*)malloc(sizeof(tcb_t));
    
    for (int i = 0; i < 17; ++i) {
        new_tcb->regs[i] = 0; // Inicializa os registradores
    }
    
    new_tcb->tid = tid;
    new_tcb->priority = priority;
    new_tcb->exc_slots = exc_slots;
    new_tcb->age = age;
    
    return new_tcb;
}