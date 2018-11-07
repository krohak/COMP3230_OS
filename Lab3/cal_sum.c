#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define METHOD 1

/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
	int tid;
	long long limit;
	int num_threads;
	long long answer;
} thread_data_t;

/* thread function */
void *thr_func(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;
	int myID = data->tid;
	long long workload =  data->limit/data->num_threads + 1;
	long long start = myID * workload;
	long long end = start + workload > data->limit ? data->limit + 1: start + workload;
	long long my_sum = 0;
	long long i;
	for(i = start; i < end; i++) {
		my_sum += i;
	}
//	printf("thread[%d]:workload: %lld, start: %lld, end: %lld, thread_sum: %lld\n", 
//			data->tid, workload, start, end, my_sum);

#if METHOD == 1 	
	//*** Method 1: passing in the answer pointer ***//
	data->answer = my_sum; 
	pthread_exit(NULL);
#else
	//*** Method 2: dynamic allocate memory ***//
	long long *answer = malloc(sizeof(*answer));
	*answer = my_sum;
	pthread_exit(answer);
#endif
}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: %s <num to cal> <NUM_THREADS>\n", argv[0]);
		return EXIT_SUCCESS;
	}
	long long limit = atoll(argv[1]);
	int NUM_THREADS = atoi(argv[2]);
	printf("Using %d threads to calculate the sum form 1 to %lld.\n", NUM_THREADS, limit);
	pthread_t thr[NUM_THREADS];
	int i, rc;
	thread_data_t thr_data[NUM_THREADS];
	double timer = omp_get_wtime();
	/* create threads */
	for (i = 0; i < NUM_THREADS; ++i) {
		thr_data[i].tid = i;
		thr_data[i].limit = limit;
		thr_data[i].num_threads = NUM_THREADS;
		if ((rc = pthread_create(&thr[i], NULL, thr_func, &thr_data[i]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}
	long long sum = 0;
#if METHOD == 1
	//*** Method 1 ***//
	for (i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], NULL);
		sum += thr_data[i].answer;
	}
#else
	//*** Method 2 ***//
	long long *thread_sum;
	for (i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], (void**) &thread_sum);
		sum += *thread_sum;
	}
#endif
	timer = omp_get_wtime() - timer;
	printf("sum is : %lld, time use: %f\n", sum, timer); 
	return EXIT_SUCCESS;
}

