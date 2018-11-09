#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


void *producer(void *arg);

void *consumer(void *arg);

typedef struct sem_pack {
	int tid;
	sem_t *item;
	sem_t *space;
} sem_pack;
