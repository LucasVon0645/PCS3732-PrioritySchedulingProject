#define MAX_QUEUE_SIZE 10

// Estrutura da fila dos PIDs
typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

int isEmpty(Queue* queue);
int isFull(Queue* queue);

int enqueue(Queue* queue, int item);
int dequeue(Queue* queue);

int front(Queue* queue);
int rear(Queue* queue);