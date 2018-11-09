#include "thread_func.h"

void *producer(void *arg) {
	//Unpack thread pack
	
	sem_pack * pack = (sem_pack*) arg;
	int tid = pack->tid;
	sem_t *item = pack->item;
	sem_t *space = pack->space;
	

	int *num_sem = malloc(sizeof(num_sem)); 
	sem_getvalue(space, num_sem);
	printf("th[%d] before wait space(%p), %d\n", tid, space, *num_sem);
	// wait for space
	sem_wait(space);

	sem_getvalue(space, num_sem);
	printf("th[%d] space(%p) get, %d\n", tid, space, *num_sem);

	sem_getvalue(item, num_sem);
	printf("th[%d] before post item(%p), %d\n", tid, item, *num_sem); 
	printf("th[%d] Producing stuff...\n", tid);
	sleep(2);  //simulating producing process
	//Production Finish, post to item
	sem_post(item);

	sem_getvalue(item, num_sem);
	printf("th[%d] item(%p) posted, %d\n", tid, item, *num_sem); 
	free(num_sem);
	pthread_exit(0);
}

void *consumer(void *arg) {
	//Unpack thread pack
	
	sem_pack * pack = (sem_pack*) arg;
	int tid = pack->tid;
	sem_t *item = pack->item;
	sem_t *space = pack->space;


	int *num_sem = malloc(sizeof(num_sem)); 
	sem_getvalue(item, num_sem);
	printf("th[%d] before wait item(%p), waiting for stuff.. %d\n", tid, item, *num_sem);
	//Wait for item
	sem_wait(item);

	sem_getvalue(item, num_sem);
	printf("th[%d] sem(%p) get item, %d\n", tid, item, *num_sem);


	sem_getvalue(space, num_sem);
	printf("th[%d] before post space(%p), %d\n", tid, space, *num_sem);
	// Item get, release space by post to space
	sem_post(space);

	sem_getvalue(space, num_sem);
	printf("th[%d] space(%p) posted, %d\n", tid, space, *num_sem);

	free(num_sem);
	pthread_exit(0);
}

