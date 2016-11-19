#include "shared_mem.h"
#include <unistd.h>
#include <string.h>

int fd;
int errno;
struct shared_mem* shared_mem;

int setup_shared_memory(){
	fd = shm_open(MY_SHM, O_CREAT | O_RDWR, S_IRWXU);
	if(fd == -1){
		printf("shm_open() failed\n");
		exit(1);
	}
	ftruncate(fd, sizeof(struct shared_mem));
}

int attach_shared_memory(){
	shared_mem = (struct shared_mem*)  mmap(NULL, sizeof(struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_mem == MAP_FAILED){
		printf("mmap() failed\n");
		exit(1);
	}

	return 0;
}

int init_shared_memory() {
	printf("Memory not initialized\nEnter number of slots:");

	int size;
	int p = scanf("%u", &size);

	printf("%d\n", size);


	if(p != 1) {
		printf("Input failure\n");
		exit(1);

	} else if(size > MAX_SLOTS) {
		printf("Size exceeded maximum (%d), defaulting to %d.\n", MAX_SLOTS, MAX_SLOTS);
		size = MAX_SLOTS;		
	}


	shared_mem->is_initialized = INIT_CONST;

	init_queue(&(shared_mem->queue), size);
	sem_init(&(shared_mem->sem_mutex), 1, 1);
}

int main(int argc, char *argv[]) {
	setup_shared_memory();
	attach_shared_memory();

	int not_initialized = shared_mem->is_initialized != INIT_CONST;
	if((argc >= 2 && (strcmp(argv[1], "renew") == 0)) || not_initialized) {
		init_shared_memory();
	} else {
		printf("Memory already initialized: connected to bounded buffer of size %d\n", shared_mem->queue.size);
		printf("To reconfigure the shared memory, run './printer renew'.\n");
	}

	printf("Waiting for jobs.\n");
	
	while (1) {
		struct job my_job = dequeue(&shared_mem->queue, &shared_mem->sem_mutex);

		printf("Printing: %s\n", my_job.message);
		sleep(3);
		printf("Done!\n");
	}

	return 0;
}
