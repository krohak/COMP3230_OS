#include "definitions.h"
#include "main.h"
#include <omp.h>

sem_t sem_worker;
sem_t sem_space;

sem_t sem_skeleton;
sem_t sem_engine;
sem_t sem_chassis;
sem_t sem_body;

sem_t sem_window;
sem_t sem_tire;
sem_t sem_battery;
sem_t sem_car;

int num_cars;
int num_spaces;
int num_workers;

#define WORK_GROUP  8

int main(int argc, char** argv)
{	
	printf("Name: Rohak Singhal\tUID: 3035242475\n");

	if (argc < 4) {
	printf("Usage: %s <number of cars> <number of spaces> <number of workers>\n", 
	argv[0]);
	return EXIT_SUCCESS;
	}
	num_cars     = atoi(argv[1]);
	num_spaces   = atoi(argv[2]);
	num_workers  = atoi(argv[3]);


	// We only make one car with 1 thread and sufficient storage spaces
	// num_cars     = 1; 
	// num_spaces   = 1;
	// num_workers  = 8;

	printf("Job defined, %d workers will build %d cars with %d storage spaces\n",
			num_workers, num_cars, num_spaces);


	// in the worst case ordering of jobs, we need
	// at least 16 separate spaces to avoid deadlock
	// (sum of all individual parts)
	if (num_spaces < 16){
		deadlockProgram();
	}

	else{
		basicProgram();
	}

	return EXIT_SUCCESS;
}


void basicProgram(){
	
	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));

	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);

	// prepare work_pack
	work_pack wpack[num_workers]; 

	// thread objects
	pthread_t thr[num_workers];

	// Start working and time the whole process
	double production_time = omp_get_wtime();

	for(int t_group = 0; t_group < ceil( num_cars *  WORK_GROUP / (double) num_workers); t_group++){

		int num_threads;

		if ( ( (t_group+1) * num_workers) > (num_cars *  WORK_GROUP) )
			num_threads = (num_cars *  WORK_GROUP) % num_workers;
		else
			num_threads = num_workers;

		// threads 
		for (int t = t_group*num_workers; t < (t_group*num_workers)+num_threads; t++) { 
			
			int i = t % num_workers;
			int j = t % WORK_GROUP;
			
			wpack[i].resource = rpack;
			wpack[i].tid = i;
			wpack[i].jid = j;

			// printf("-----Main: worker %d doing %d...\n", wpack[i].tid, wpack[i].jid);
			
			if (j == WINDOW)
				wpack[i].times = 7;
			else if (j == TIRE)
				wpack[i].times = 4;
			else
				wpack[i].times = 1;

			// start thread here
			int rc; 
			if (( rc = pthread_create(&thr[i], NULL, work, &wpack[i]))) {
				fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
				return;
			}

		}

		/* block until all threads complete */
		for (int t = t_group*num_workers; t < (t_group*num_workers)+num_threads; t++) {
			int i = t % num_threads;
			pthread_join(thr[i], NULL);
		}
		
	}



	production_time = omp_get_wtime() - production_time;
	reportResults(production_time);

	destroySem();
	free(rpack);
}

void deadlockProgram(){

	int job_order[] = {3, 0, 1, 2, 7, 4, 5, 6};
	
	resource_pack *rpack = (struct resource_pack*) malloc(sizeof(struct resource_pack));

	// put semaphores into resource_pack
	initResourcePack(rpack, num_spaces, num_workers);

	// prepare work_pack
	work_pack wpack[WORK_GROUP]; 

	// thread objects
	pthread_t thr[2];

	// Start working and time the whole process
	double production_time = omp_get_wtime();

	// prepare workpacks
	for (int i=0; i<WORK_GROUP; i++){
		
		wpack[i].resource = rpack;
		wpack[i].jid = job_order[i];

		if (job_order[i] == WINDOW)
			wpack[i].times = 7;
		else if (job_order[i] == TIRE)
			wpack[i].times = 4;
		else
			wpack[i].times = 1;
	}

	int rc; 

	for (int car = 0; car < num_cars; car++){

		// do job 3 first using a thread, wpack[0]
		wpack[0].tid = 0;
		if (( rc = pthread_create(&thr[0], NULL, work, &wpack[0]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return;
		}

		// do job 0,1,2 using the remaining threads
		for (int i = 1; i < 4; i++){

			wpack[i].tid = 1;
			if (( rc = pthread_create(&thr[1], NULL, work, &wpack[i]))) {
				fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
				return;
			}
			
			// join 
			pthread_join(thr[1], NULL);
		}

		// join job 3
		pthread_join(thr[0], NULL);


		// do job 7 first using a thread, wpack[4]
		wpack[4].tid = 0;
		if (( rc = pthread_create(&thr[0], NULL, work, &wpack[4]))) {
			fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
			return;
		}


		// do job 4,5,6 using the remaining threads
		for (int i = 5; i < 8; i++){

			wpack[i].tid = 1;
			if (( rc = pthread_create(&thr[1], NULL, work, &wpack[i]))) {
				fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
				return;
			}
			
			// join 
			pthread_join(thr[1], NULL);
		}

		
		// join 7
		pthread_join(thr[0], NULL);
	}

	production_time = omp_get_wtime() - production_time;
	reportResults(production_time);

	destroySem();
	free(rpack);
}


void reportResults(double production_time) {
	int *sem_value = malloc(sizeof(int));
	printf("=====Final report=====\n");

	sem_getvalue(&sem_skeleton, sem_value);
	printf("Unused Skeleton: %d\n",   *sem_value);
	sem_getvalue(&sem_engine,   sem_value);
	printf("Unused Engine: %d\n",     *sem_value);
	sem_getvalue(&sem_chassis,  sem_value);
	printf("Unused Chassis: %d\n",    *sem_value);
	sem_getvalue(&sem_body,     sem_value);
	printf("Unused Body: %d\n",       *sem_value);
	sem_getvalue(&sem_window,   sem_value);
	printf("Unused Window: %d\n",     *sem_value);
	sem_getvalue(&sem_tire,     sem_value);
	printf("Unused Tire: %d\n",       *sem_value);
	sem_getvalue(&sem_battery,  sem_value);
	printf("Unused Battery: %d\n",    *sem_value);

	sem_getvalue(&sem_space, sem_value);
	if (*sem_value < num_spaces) {
		printf("There are waste car parts: %d!\n", *sem_value);
	}
	sem_getvalue(&sem_car, sem_value);
	printf("Production of %d %s done, production time: %f sec, space usage: %d\n", 
			*sem_value,
			*sem_value > 1 ? "cars" : "car",	       
			production_time, num_spaces);
	printf("==========\n");
	free(sem_value);
}

void initResourcePack(struct resource_pack *pack,
		int space_limit, int num_workers) {
	initSem();
	pack->space_limit  = space_limit;
	pack->num_workers  = num_workers;
	pack->sem_space    = &sem_space   ;
	pack->sem_worker   = &sem_worker  ;

	pack->sem_skeleton = &sem_skeleton;
	pack->sem_engine   = &sem_engine  ;
	pack->sem_chassis  = &sem_chassis ;
	pack->sem_body     = &sem_body    ;

	pack->sem_window   = &sem_window  ;
	pack->sem_tire     = &sem_tire    ;
	pack->sem_battery  = &sem_battery ;
	pack->sem_car      = &sem_car     ;
}

int destroySem(){
#if DEBUG
	printf("Destroying semaphores...\n");
#endif
	sem_destroy(&sem_worker);
	sem_destroy(&sem_space);

	sem_destroy(&sem_skeleton);
	sem_destroy(&sem_engine);
	sem_destroy(&sem_chassis);
	sem_destroy(&sem_body);

	sem_destroy(&sem_window);
	sem_destroy(&sem_tire);
	sem_destroy(&sem_battery);
	sem_destroy(&sem_car);
#if DEBUG
	printf("Semaphores destroyed\n");
#endif
	return 0;
}

int initSem(){
#if DEBUG
	printf("Initiating semaphores...\n");
#endif
	sem_init(&sem_worker,   0, num_workers);
	sem_init(&sem_space,    0, num_spaces);

	sem_init(&sem_skeleton, 0, 0);
	sem_init(&sem_engine,   0, 0);
	sem_init(&sem_chassis,  0, 0);
	sem_init(&sem_body,     0, 0);

	sem_init(&sem_window,   0, 0);
	sem_init(&sem_tire,     0, 0);
	sem_init(&sem_battery,  0, 0);
	sem_init(&sem_car,      0, 0);
#if DEBUG
	printf("Init semaphores done!\n");
#endif
	return 0;
}

