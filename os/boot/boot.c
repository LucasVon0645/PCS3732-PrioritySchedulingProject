#include "boot.h"

void boot() {
    tcb_t *main_tcb = create_tcb(0, 0, 2, 0, (uint32_t)main2);

    tcb_t* tcb1 = create_tcb(1, 0, 2, 0, (uint32_t)main2);
    tcb_t* tcb2 = create_tcb(2, 1, 5, 0, (uint32_t)main2);
    tcb_t* tcb3 = create_tcb(3, 1, 5, 0, (uint32_t)main2);
    tcb_t* tcb4 = create_tcb(4, 2, 10, 0, (uint32_t)main);
    tcb_t* tcb5 = create_tcb(5, 2, 10, 0, (uint32_t)main);

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

    enqueue(queue0, main_tcb);
    enqueue(queue0, tcb1);
    enqueue(queue1, tcb2);
    enqueue(queue1, tcb3);
    enqueue(queue2, tcb4);
    enqueue(queue2, tcb5);

    update_next_thread(&multi_queue);

    mfqs_scheduler();
}

// Função para criar um elemento tcb_t
tcb_t* create_tcb(uint32_t tid, uint32_t priority, uint32_t exc_slots, uint32_t age, uint32_t entry_point) {
    tcb_t* new_tcb = (tcb_t*)malloc(sizeof(tcb_t));
    uint8_t* stack = (uint8_t*)malloc(sizeof(uint8_t) * 4096);
    
    for (int i = 0; i < 17; ++i) {
        new_tcb->regs[i] = 0; // Inicializa os registradores
    }

    new_tcb->regs[13] = (uint32_t)(stack + 4096);
    new_tcb->regs[14] = (uint32_t)halt;
    new_tcb->regs[15] = entry_point;
    
    new_tcb->tid = tid;
    new_tcb->priority = priority;
    new_tcb->exc_slots = exc_slots;
    new_tcb->age = age;
    new_tcb->cpu_time = 0;
    
    return new_tcb;
}