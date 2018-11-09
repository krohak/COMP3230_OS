#include "thread_func.h"

sem_t sem_item;
sem_t sem_space;

int main(int argc, char **argv) {
	int num_threads = 2;

	sem_init(&sem_item, 0, 0);
	sem_init(&sem_space, 0, 1);

	int *num_sem = malloc(sizeof(num_sem));
	sem_getvalue(&sem_item, num_sem);
	printf("Initial item value %d\n", *num_sem);
	sem_getvalue(&sem_space, num_sem);
	printf("Initial space value %d\n", *num_sem);

	sem_pack packs[num_threads];
	pthread_t th[num_threads];

	for (int i = 0; i < num_threads; i++) {
		// Pack thread id and semaphores
		packs[i].tid = i;
		packs[i].item = &sem_item;
		packs[i].space = &sem_space;
	}

	// Create producer thread
	pthread_create(&th[0], NULL, producer, &packs[0]);

	// Create consumer thread
	pthread_create(&th[1], NULL, consumer, &packs[1]);

	for (int i = 0; i < num_threads; i++) {
		pthread_join(th[i], NULL);
	}

	sem_destroy(&sem_item);
	sem_destroy(&sem_space);
	free(num_sem);
	return 0;
}
