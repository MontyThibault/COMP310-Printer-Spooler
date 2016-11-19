#include "common.h"
#include "buffer_queue.h"

#define MY_SHM "/JIT"
#define INIT_CONST 0x12345678

struct shared_mem {
	int is_initialized;

	struct queue queue;
	sem_t sem_mutex;
};
