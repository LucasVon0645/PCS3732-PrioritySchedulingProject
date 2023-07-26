
#include <stdint.h>

#define MAX_QUEUE_SIZE 10
#define NUM_OF_QUEUES 3

// Definidos pelo linker:
extern uint8_t stack_usr1[];
extern uint8_t stack_usr2[];


// Estrutura do Thread control block (TCB).
typedef struct {
   uint32_t pid;        // identificador do processo
   uint32_t regs[17];   // contexto (17 registradores)
   uint32_t priority;   // prioridade atual do processo
   uint32_t exc_slots;  // numero de execucoes
} tcb_t;

// Estrutura da fila dos PIDs
typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Function to create an empty queue
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

// Function to check if the queue is empty
int isEmpty(Queue* queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue* queue) {
    return ((queue->rear + 1) % MAX_QUEUE_SIZE == queue->front);
}

// Function to add an element to the queue
int enqueue(Queue* queue, int item) {
    if (isFull(queue)) {
        return 0;
    }

    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    }

    queue->data[queue->rear] = item;
    return 1;
}

// Function to remove an element from the queue
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }

    int item = queue->data[queue->front];

    if (queue->front == queue->rear) {
        queue->front = -1;
        queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    }

    return item;
}

// Function to get the front element of the queue without removing it
int front(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }

    return queue->data[queue->front];
}

// Function to get the rear element of the queue without removing it
int rear(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }

    return queue->data[queue->rear];
}

// Function to free the memory allocated to the queue
void destroyQueue(Queue* queue) {
    free(queue);
}


// Pontos de entrada dos threads
int main(void);
int main2(void);


// Mock de threads
tcb_t p1 = {
   1,
   {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
      (uint32_t)stack_usr1,                    // sp
      0,                                       // lr inicial
      (uint32_t)main,                          // pc = lr = ponto de entrada
      0x10,                                    // valor do cpsr (modo usuário)
   },
   5
};

tcb_t p2 = {
   2,
   {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
      (uint32_t)stack_usr2,                    // sp
      0,                                       // lr inicial
      (uint32_t)main2,                         // pc = lr = ponto de entrada
      0x10,                                    // valor do cpsr (modo usuário)
   },
   5
};

tcb_t p3 = {
   3,
   {
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // r0-r12
      (uint32_t)stack_usr2,                    // sp
      0,                                       // lr inicial
      (uint32_t)main2,                         // pc = lr = ponto de entrada
      0x10,                                    // valor do cpsr (modo usuário)
   },
   5
};

/**
 * Chama o kernel com swi, a função "yield" (r0 = 1).
 * Devolve o controle ao sistema executivo, que pode escalar outro thread.
 */
void __attribute__((naked)) yield(void) {
   asm volatile("push {lr}  \n\t"
                "mov r0, #1 \n\t"
                "swi #0     \n\t"
                "pop {pc}");
}


Queue* queue0 = createQueue();
Queue* queue1 = createQueue();
Queue* queue2 = createQueue();
Queue* multilevel_queue[3] = {queue0, queue1, queue2};

void ageAllThreads() {}

int thread_scheduler() {
   Queue* next_nonempty_queue;
   for(int i = 0; i < NUM_OF_QUEUES; i++) {
      Queue* queue_i = multilevel_queue[i];
      next_nonempty_queue = !isEmpty(queue_i) ? queue_i : next_nonempty_queue;
   }
   return dequeue(next_nonempty_queue);
}

void update_executed_thread(tcb_t* cur_thread) {
   int pid = cur_thread->pid;
   int priority = cur_thread->priority;
   int remaining_quantums = cur_thread->exc_slots - 1;
   Queue* current_queue = multilevel_queue[priority];

   if (remaining_quantums == 0 && priority < NUM_OF_QUEUES - 1) {
      Queue* next_queue = multilevel_queue[priority];
      dequeue(current_queue);
      enqueue(next_queue, pid);
   }
}

tcb_t tcb_array[3] = {p1, p2, p3};
volatile void *curr_tcb = &tcb_array[0];  // tcb do thread atual

void mfqs(void) {
   update_executed_thread(curr_tcb);
   int next_tid = thread_scheduler();
   curr_tcb = &tcb_array[next_tid];
}