#pragma once
#include <stdint.h>

typedef struct {
    uint32_t regs[17];   // contexto (17 registradores)
    uint32_t tid;        // identificador da thread
    uint32_t priority;   // prioridade atual da thread
    uint32_t exc_slots;  // numero de execucoes restantes
    uint32_t age;        // numero de execucoes restantes
    uint32_t cpu_time;   // quantidade total de execucoes de slots de tempo da thread
    uint8_t stack_padding; // ajuste, para o stack estar alinhado na tcb
    uint8_t stack[4096];   // stack
} tcb_t;