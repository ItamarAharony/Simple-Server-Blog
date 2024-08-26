#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    int data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue *create_queue();
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
int is_empty(Queue *queue);

#endif

