#include "threads.h"
#define stack_start(_tcb) ((uint32_t*)(_tcb + sizeof(tcb_t) - sizeof(uint8_t)))

int latest_tid;

// Pega proximo tid global
uint32_t get_next_tid(void) {
    return latest_tid++;
}

void* aligned_malloc(size_t required_bytes, size_t alignment) {
    void* p1;
    void** p2;
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(required_bytes + offset)) == NULL) {
      return NULL;
    }
    p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void *p) {
    free(((void**)p)[-1]);
}

// Função para criar um elemento tcb_t
tcb_t* create_tcb(
    uint32_t priority,
    uint32_t exc_slots,
    uint32_t entry_point,
    uint32_t exit_point
) {
    tcb_t* new_tcb = (tcb_t*)aligned_malloc(sizeof(tcb_t), 8);
    
    // Inicializa os registradores
    for (int i = 0; i < 17; i++)
        new_tcb->regs[i] = 0;
    new_tcb->regs[15] = entry_point;
    new_tcb->regs[14] = (uint32_t)exit_point;
    new_tcb->regs[13] = stack_start(new_tcb);

    // Inicializa outros atributos da TCB
    new_tcb->tid = get_next_tid();
    new_tcb->priority = priority;
    new_tcb->exc_slots = exc_slots;
    new_tcb->age = 0;
    new_tcb->cpu_time = 0;
    
    return new_tcb;
}


// Função para criar um novo elemento tcb_t a partir de uma copia
tcb_t* copy_tcb(tcb_t* original_tcb) {
    // Aloca espaco para uma nova tcb
    tcb_t* new_tcb = (tcb_t*)aligned_malloc(sizeof(tcb_t), 8);

    // Copia a tcb original
    *new_tcb = *original_tcb;

    // Modificamos sp para apontar para o novo stack
    uint32_t sp_offset = stack_start(original_tcb) - original_tcb->regs[13];
    new_tcb->regs[13] = stack_start(new_tcb) - sp_offset;

    // Modifica tid da thread
    new_tcb->tid = get_next_tid();
    
    return new_tcb;
}
