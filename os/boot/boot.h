#pragma once
#include "../threads/threads.h"
#include "../scheduler/scheduler.h"
#include "../syscalls/syscalls.h"

extern multiqueue_t multi_queue;
extern int latest_tid;

// Pontos de entrada dos threads
int main(void);
int main2(void);

