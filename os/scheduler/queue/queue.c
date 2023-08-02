#include "queue.h"

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