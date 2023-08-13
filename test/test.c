#include <stdio.h>
#include "test.h"

int main()
{
    printf("TEST BEGIN!\n");
    test_boot();
    printf("BOOT END!\n");

    int change_context = 0;
    for (int i = 0; i < 4; i++) {
        printf("\n Update threads %d \n", i);
        change_context = mfqs_update_threads(0);
        printf("\n Before fork %d \n", i);
        print_multi_queue(&multi_queue);
        _fork();
        printf("\n After fork %d \n", i);
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


void test_boot() {
    latest_tid = 1;

    tcb_t *main_tcb = create_tcb(0, 2, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb1 = create_tcb(0, 2, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb2 = create_tcb(1, 5, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb3 = create_tcb(1, 5, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb4 = create_tcb(2, 10, (uint32_t)main, (uint32_t)halt);
    tcb_t* tcb5 = create_tcb(2, 10, (uint32_t)main, (uint32_t)halt);

    for (int i = 0; i < 6; i++) main_tcb->stack[i] = i;
    for (int i = 0; i < 6; i++) tcb1->stack[i] = i+10;
    for (int i = 0; i < 6; i++) tcb2->stack[i] = i+20;

    queue_t *queue0 = (queue_t*)malloc(sizeof(queue_t));
    queue0->quanta_limit = 2;
    queue0->age_limit = 5;
    queue0->head = NULL;

    queue_t *queue1 = (queue_t*)malloc(sizeof(queue_t));
    queue1->quanta_limit = 5;
    queue1->age_limit = 10;
    queue1->head = NULL;

    queue_t *queue2 = (queue_t*)malloc(sizeof(queue_t));
    queue2->quanta_limit = 10;
    queue2->age_limit = 30;
    queue2->head = NULL;

    multi_queue.queues[0] = queue0;
    multi_queue.queues[1] = queue1;
    multi_queue.queues[2] = queue2;

    add_to_multiqueue(main_tcb, &multi_queue);
    add_to_multiqueue(tcb1, &multi_queue);
    add_to_multiqueue(tcb2, &multi_queue);
    add_to_multiqueue(tcb3, &multi_queue);
    add_to_multiqueue(tcb4, &multi_queue);
    add_to_multiqueue(tcb5, &multi_queue);

    update_next_thread(&multi_queue);
    mfqs_scheduler();
}

void halt() {
    return;
}

void main1() {
    return;
}

void main2() {
    return;
}



void print_tcb(tcb_t* tcb) {
    printf(" Thread ID: %u\n", tcb->tid);
    printf(" Priority: %u\n", tcb->priority);
    printf(" Execution Slots: %u\n", tcb->exc_slots);
    printf(" Age: %u\n", tcb->age);
    printf(" Stack position: ");
    print_binary(&(tcb->stack[0]));
    printf("\n");
    printf(" First stack values:\n");
    for (int i = 0; i < 6; i++) printf("%u ", tcb->stack[i]);
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

void print_binary(int num) {
    // Size of an integer in bytes
    int numBits = sizeof(num) * 8;

    for (int i = numBits - 1; i >= 0; i--) {
        // Extract the i-th bit using bitwise right shift
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}
