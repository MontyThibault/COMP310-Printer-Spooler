#include "buffer_queue.h"

void init_queue(struct queue *queue, int size) {

	if(size > MAX_SLOTS) {
		printf("Error: buffer size too large");
		return;
	}

	queue->head = 0;
	queue->tail = 0;

	sem_init(&(queue->sem_overflow), 1, size);
	sem_init(&(queue->sem_underflow), 1, 0);

	queue->size = size;
}

void enqueue(struct queue *queue, sem_t *sem_mutex, struct job job) {
	sem_wait(&(queue->sem_overflow));
	sem_wait(sem_mutex);

	queue->head += 1;	

	int index = queue->head % queue->size;
	queue->buffer[index] = job;	

	sem_post(sem_mutex);
	sem_post(&(queue->sem_underflow));
}

struct job dequeue(struct queue *queue, sem_t *sem_mutex) {
	sem_wait(&(queue->sem_underflow));
	sem_wait(sem_mutex);
	
	queue->tail += 1;	
	int index = queue->tail % queue->size;
	struct job job = queue->buffer[index];

	sem_post(sem_mutex);
	sem_post(&(queue->sem_overflow));
	return job;	
}
