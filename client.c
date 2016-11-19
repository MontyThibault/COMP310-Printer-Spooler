#include "shared_mem.h"
#include <string.h>

int fd;
struct shared_mem *shared_mem;

int setup_shared_memory(){
	fd = shm_open(MY_SHM, O_RDWR, S_IRWXU);
	if(fd == -1){
		printf("shm_open() failed\n");
		exit(1);
	}
}

int attach_shared_memory(){
	shared_mem = (struct shared_mem*) mmap(NULL, sizeof(struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_mem == MAP_FAILED){
		printf("mmap() failed\n");
		exit(1);
	}

	return 0;
}

/*
void handler(int signo){
	int temp;
	sem_getvalue(&shared_mem->sem_binary, &temp);
	if(temp != 1)
		sem_post(&shared_mem->sem_binary);
	sem_getvalue(&shared_mem->sem_resource, &temp);
	if(temp != 1)
		sem_post(&shared_mem->sem_resource);
	exit(0);
}
*/

int main() {

	// What is this?
	// if(signal(SIGINT, handler) == SIG_ERR)
	//	printf("Signal Handler Failure ..\n");

	setup_shared_memory();
	attach_shared_memory();

	if(shared_mem->is_initialized != INIT_CONST) {
		printf("No shared memory found.\n");
		exit(1);
	}

	printf("Please type a message to print.\n");
	
	char message[200] = "";
	scanf("%79[^\n]", message);
	printf("The message was: %s\n", message);

	struct job my_job;
	strcpy(my_job.message, message);

	enqueue(&shared_mem->queue, &shared_mem->sem_mutex, my_job);

	printf("Job added to queue.\n");

	return 0;
}
