#include <stdint.h>
#include "queue/multiqueue.h"

// Definidos pelo linker
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];
extern uint8_t stack_usr3[];

// Pontos de entrada dos threads
int main(void);
int main2(void);
int main3(void);

int mfqs_scheduler();