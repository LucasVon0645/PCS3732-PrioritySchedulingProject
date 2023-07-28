#include <stdint.h>
#include "thread_table.h"

void add_to_table(thread_table* table, tcb* new_tcb) {
    node *new_node = {new_tcb, NULL, NULL};

    if (table->list_head == NULL) {
        table->list_head = new_node;
        table->list_end = new_node;
        new_node->next_node = new_node;
        new_node->previous_node = new_node;
        return;
    }

    new_node->next_node = table->list_head;
    new_node->previous_node = table->list_end;
    table->list_end->next_node = new_node;
    table->list_head->previous_node = new_node;
    table->list_end = new_node;
}

int delete_from_table_by_id(thread_table* table, int tid) {
    node* current_node = table->list_head;
    int current_tid = current_node->tcb_t->tid;

    while (current_tid != tid) {
        current_node = current_node->next_node;
        if (current_node == table->list_end) return 0;
        current_tid = current_node->tcb_t->tid;
    }

    node* previous_node = current_node->previous_node;
    node* next_node = current_node->next_node;

    // Single element in the list
    if (next_node == current_node) {
        table->list_head = NULL;
        table->list_end = NULL;

    }
    // Only two elements in the list
    else if (next_node == previous_node) {
        next_node->next_node = NULL;
        next_node->previous_node = NULL;
        table->list_head = next_node;
        table->list_end = next_node;
    }
    // Arbitrary number of elements in the list
    else {
        previous_node->next_node = next_node;
        next_node->previous_node = previous_node;
        if (table->list_head == current_node) table->list_head = next_node;
        if (table->list_end == current_node) table->list_end = previous_node;
    }

    return current_node->tcb_t->tid;
}