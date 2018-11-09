#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LOOPS 1000000
long long counter = 0;
sem_t sem;

void* counting_thread(void* arg) {
	int offset = *(int*) arg;
	int i;
	for(i = 0; i < NUM_LOOPS; i++){

		sem_wait(&sem);
		counter += offset;
		sem_post(&sem);		
	}
	pthread_exit(NULL);
}

int main() {
	sem_init(&sem, 0, 1);
	pthread_t th1;
	int offset = 1;
	pthread_create(&th1, NULL, counting_thread, &offset);

	pthread_t th2;
	int offset2 = -1;
	pthread_create(&th2, NULL, counting_thread, &offset2);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	printf("Finnal counter value: %lld\n", counter);
	return 0;
}
