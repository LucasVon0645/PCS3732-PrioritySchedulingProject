#include "boot.h"

void boot() {
    tcb_t bootloader_tcb = {
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
            (uint32_t)stack_usr1,                    // sp
            0,                                       // lr inicial
            (uint32_t)main,                          // pc = lr = ponto de entrada
            0x10,                                    // valor do cpsr (modo usuário)
        },
        0, // identificador da thread (tid)
        1, // prioridade atual da thread
        5 // numero de execucoes restantes 
    };

    tcb_t *main_tcb = (tcb_t*)malloc(sizeof(tcb_t));
    *main_tcb = bootloader_tcb;

    queue_t *queue0 = (queue_t*)malloc(sizeof(queue_t));
    queue0->quanta_limit = 2;
    queue0->age_limit = 5;

    queue_t *queue1 = (queue_t*)malloc(sizeof(queue_t));
    queue1->quanta_limit = 5;
    queue1->age_limit = 10;

    queue_t *queue2 = (queue_t*)malloc(sizeof(queue_t));
    queue2->quanta_limit = 10;
    queue2->age_limit = 20;

    multi_queue.queues[0] = queue0;
    multi_queue.queues[1] = queue1;
    multi_queue.queues[2] = queue2;
    enqueue(queue0, main_tcb);
    update_next_thread(&multi_queue);

    mfqs_scheduler();
}