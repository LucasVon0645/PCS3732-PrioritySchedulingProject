#include <stdint.h>

#define MAX_QUEUE_SIZE 10
#define NUM_OF_QUEUES 3

// Estrutura da fila dos PIDs
typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

typedef struct {
    Queue* queues[NUM_OF_QUEUES];
    int current_queue;
    int current_queue_priority;
} MultilevelQueue;

int isEmpty(Queue* queue);
int isFull(Queue* queue);

int enqueue(Queue* queue, int item);
int dequeue(Queue* queue);

int front(Queue* queue);
int rear(Queue* queue);