#pragma once
#include "../threads/threads.h"
#include "../scheduler/scheduler.h"
#include "../syscalls/syscalls.h"

extern multiqueue_t multi_queue;
extern tcb_t* os_tcb;
extern int latest_tid;

// Pontos de entrada dos threads
int os_thread(void);
int user_thread(void);


