#pragma once
#include "../os/os.h"

extern int latest_tid;

void test_boot();
void print_multi_queue(multiqueue_t* multiqueue);
void print_queue(queue_t* queue);
void print_tcb(tcb_t* tcb);
void print_binary();

void halt();
void main1();
void main2();