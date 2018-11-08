#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>


/* create thread argument struct for thr_func() */
typedef struct _thread_data_t {
	int tid;
	long long limit;
	int num_threads;
	double answer;
} thread_data_t;


/* thread function */
void *thr_func(void *arg) {
	thread_data_t *data = (thread_data_t *)arg;
	int myID = data->tid;
	long long workload =  data->limit/data->num_threads + 1;
	long long start = myID * workload;
	long long end = start + workload > data->limit ? data->limit + 1: start + workload;
	double step = 1.0/(double) data->limit;

	double my_sum = 0;
	long long i;

	// main loop to calculate area under the graph from start x to end x
	 for (i = start; i< end; i++){
		  double x = (i-0.5)*step;
		  my_sum = my_sum + 4.0/(1.0+x*x);
	  }

	my_sum = step * my_sum;

	// printf("thread[%d]:workload: %lld, start: %lld, end: %lld, thread_sum: %lf\n", 
	// 		data->tid, workload, start, end, my_sum);

	data->answer = my_sum; 
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	
	if (argc < 3) {
		printf("Usage: %s <num iterations> <NUM_THREADS>\n", argv[0]);
		return EXIT_SUCCESS;
	}
	long long limit = atoll(argv[1]);
	int NUM_THREADS = atoi(argv[2]);
	
	printf("pi with %lld steps:\n", limit);

	// printf("Using %d threads to calculate pi with %lld iterations.\n", NUM_THREADS, limit);
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

	double sum = 0;

	for (i = 0; i < NUM_THREADS; ++i) {
		pthread_join(thr[i], NULL);
		sum += thr_data[i].answer;
	}

	timer = omp_get_wtime() - timer;
	printf("\tthreads : %d, pi is : %lf, %f seconds\n", NUM_THREADS, sum, timer); 
	return EXIT_SUCCESS;
}

