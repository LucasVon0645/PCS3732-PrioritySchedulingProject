#include <stdint.h>
#include "scheduler.h"


queue_t queue0 = {NULL, 2, 0};
queue_t queue1 = {NULL, 5, 30};
queue_t queue2 = {NULL, 8, 50};
multiqueue_t multi_queue = {{&queue0, &queue1, &queue2}, NULL};
volatile tcb_t *current_tcb;


int mfqs_update_threads() {
    // Atualiza thread que acabou de ser executado
    if (!(--current_tcb->exc_slots))
        downgrade_thread(current_tcb, &multi_queue);

    // Faz o aging de todas as threads, exceto a que executou agora
    age_all_threads(&multi_queue);
    current_tcb->age = 0;

    // Atualiza a proxima thread a ser executada
    update_next_thread(&multi_queue);

    // Checa se precisa salvar contexto
    return (current_tcb == multi_queue.next_thread) ? 0 : 1;
}

void mfqs_scheduler() {
    // Pega a nova thread a ser executada
    tcb_t *next_tcb = multi_queue.next_thread;

    // Checa se precisa atualizar a current_tcb global
    if (current_tcb != next_tcb) {
        next_tcb->age = 0;
        current_tcb = next_tcb;
    }
}