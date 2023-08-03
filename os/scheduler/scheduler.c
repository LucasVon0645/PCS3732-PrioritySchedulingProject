#include <stdint.h>
#include "scheduler.h"

multiqueue_t multi_queue = {{{NULL, 2, 0}, {NULL, 5, 30}, {NULL, 8, 50}}};
volatile tcb_t *current_tcb;

int mfqs_scheduler() {
    // Atualiza thread que acabou de ser executado
    if (!(--current_tcb->exc_slots))
        downgrade_thread(current_tcb, &multi_queue);

    // Faz o aging de todas as threads, exceto a que executou agora
    age_all_threads(&multi_queue);
    current_tcb->age = 0;

    // Pega uma nova thread a ser executada
    tcb_t *next_tcb = highest_nonempty_queue_head(&multi_queue);

    // Checa se precisa atualizar a current_tcb global
    if (current_tcb != next_tcb) {
        next_tcb->age = 0;
        current_tcb = next_tcb;
        return 1;
    }
    return 0;
}