#ifndef _INCLUDE_BUFFER_QUEUE_H_
#define _INCLUDE_BUFFER_QUEUE_H_

#include "common.h"

#define MAX_SLOTS 100

typedef struct job queue_buffer[MAX_SLOTS];

struct queue {
	queue_buffer buffer;
	int size;

	int head;
	int tail;

	sem_t sem_overflow;
	sem_t sem_underflow;
};

void init_queue(struct queue *queue, int size);
void enqueue(struct queue *queue, sem_t *sem_mutex, struct job job);
struct job dequeue(struct queue *queue, sem_t *sem_mutex);

#endif // _INCLUDE_BUFFER_QUEUE_H_
