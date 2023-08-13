#include "boot.h"

void boot() {
    latest_tid = 1;

    tcb_t *main_tcb = create_tcb(0, 2, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb1 = create_tcb(0, 2, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb2 = create_tcb(1, 5, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb3 = create_tcb(1, 5, (uint32_t)main2, (uint32_t)halt);
    tcb_t* tcb4 = create_tcb(2, 10, (uint32_t)main, (uint32_t)halt);
    tcb_t* tcb5 = create_tcb(2, 10, (uint32_t)main, (uint32_t)halt);

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
