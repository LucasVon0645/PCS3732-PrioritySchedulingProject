#include <stdint.h>
#include "scheduler.h"

multiqueue_t multi_queue;
tcb_t *current_tcb;
volatile uint32_t tid;

int mfqs_update_threads(int yield) {
    current_tcb->cpu_time++; // Incrementa o tempo de cpu (qtde total de execuções de slots de tempo)

    if (yield) {
        // Mantém a thread na msm fila, caso um yield tenha sido executado
        keep_thread_on_same_queue(current_tcb, &multi_queue);
    }
    else if (!(--current_tcb->exc_slots)) {
        //  Diminui a prioridade da thread que acabou de ser executada
        downgrade_thread(current_tcb, &multi_queue);
    }

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
        tid = current_tcb->tid;
    }
}

int get_current_priority() {
    int p = current_tcb->priority;
    return p;
}

int get_current_cpu_time() {
    int cpu_time = current_tcb->priority;
}

void finish_current_thread() {
    remove_thread_from_multiqueue(current_tcb, &multi_queue);

    // Faz o aging de todas as threads, exceto a que executou agora
    age_all_threads(&multi_queue);

    // Atualiza a proxima thread a ser executada
    update_next_thread(&multi_queue);
}