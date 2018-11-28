/*
   This program will numerically compute the integral of

   4/(1+x*x) 

   from 0 to 1.  The value of this integral is pi -- which 
   is great since it gives us an easy way to check the answer.

   The is the original sequential program.  It uses the timer
   from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.

*/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <omp.h>

typedef struct _thread_data_t {
	int tid;
	int num_threads;
	long num_steps;
} thread_data_t;

void *cal_pi(void *arg) {
	thread_data_t *data = (thread_data_t *) arg;
	int myID = data -> tid;
	int num_threads = data -> num_threads;
	long thread_steps = data -> num_steps / num_threads;
	double step = 1.0/(double) data -> num_steps;
	long i;
	double x = 0.0;
	double *thread_sum = malloc(sizeof(*thread_sum)); 

	int start = myID*thread_steps;
	int end = start + thread_steps > data->num_steps ? data->num_steps:start + thread_steps;
	for (i=start; i<= end; i++){
		x = (i+0.5)*step;
		*thread_sum = *thread_sum + 4.0/(1.0+x*x);
	}
	pthread_exit(thread_sum);
}

int main (int argc, char **argv)
{
	if(argc < 3) {
		printf("Usage: %s <num_steps> <num_threads>\n", argv[0]);
		return EXIT_SUCCESS;
	}
	int i, rc;
	double pi, sum = 0.0;
	double start_time, run_time;
	long num_steps = atol(argv[1]);
	int num_threads = atoi(argv[2]);
	thread_data_t thr_data[num_threads];
	pthread_t threads[num_threads];
	start_time = omp_get_wtime();
	for(i = 0; i < num_threads; i++){
		thr_data[i].tid = i;
		thr_data[i].num_threads = num_threads;
		thr_data[i].num_steps = num_steps;
		if ((rc = pthread_create(&threads[i], NULL, cal_pi, &thr_data[i]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	double *thread_sum;
	for(i = 0; i < num_threads; i++){
		pthread_join(threads[i], (void**) &thread_sum);
		sum += *thread_sum;
	}
	pi =  sum/num_steps;
	run_time = omp_get_wtime() - start_time;
	printf("pi with %ld steps:\n\t %d threads, pi is %0.7lf, %lf seconds\n",num_steps,num_threads, pi,run_time);

	return EXIT_SUCCESS;
}	  

