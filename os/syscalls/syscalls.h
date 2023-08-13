#pragma once

// Chamadas de syscall
void yield();
void halt();
int fork();
int get_pid();
int get_priority();
int get_cpu_time();
