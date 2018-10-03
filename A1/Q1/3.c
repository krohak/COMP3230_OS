#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void checkInputs(int *num_children, int *num_tasks, int *task_per_child){


	while( *num_children<1 || *num_children>4 || *num_tasks<1 || *num_tasks>10000 || *task_per_child<1 || *task_per_child>10){

		printf("Invalid Input!!!\n");

		if(*num_children<1 || *num_children>4){
			printf("Please input a a valid integer for the number of Child Processes (1-4) >\n");
			scanf("%d",num_children);
		}

		if(*num_tasks<1 || *num_tasks>10000){
			printf("Please input a a valid integer for the number of Tasks to be done (1-10000) >\n");
			scanf("%d",num_tasks);
		}		

		if(*task_per_child<1 || *task_per_child>10){
			printf("Please input a a valid integer for the number of Tasks to be handled by a child process (1-10) >\n");
			scanf("%d",task_per_child);
		}
	}
	
}

int main() {

    printf("COMP3230B Principles of Operating Systems\t Assignment 1\t SINGHAL Rohak 3035242475\n");
	printf("Please input a command >");

	int num_children, num_tasks, task_per_child;
	
	scanf("%d",&num_children);
	scanf("%d",&num_tasks);
	scanf("%d",&task_per_child);
	
	checkInputs(&num_children, &num_tasks, &task_per_child);
	printf("Start distributing %d task(s) to %d child process(es)\n", num_tasks, num_children);

	pid_t pids[num_children];
	pid_t mainpid=getpid();

	int i = 0;
	for(i; i<num_children; i++){

		if((pids[i]=fork())==0){
			break; //each child does not have any children
		}
	}


	if (getpid()==mainpid ) { // main process
		
		
		
		for(int i=0;i<num_children;i++){

			int status;
			waitpid(pids[i],&status,0);
		}

		for(int i=0;i<num_children;i++){

			printf("%d %d\n", i, pids[i]);
		}

		printf("PARENT\n"); 
        
	}

	else {
		
		pid_t prev_pid = i>0? pids[i-1] : pids[num_children-1];
		printf("%d %d ? \n",prev_pid, getpid());
    	exit(0);
	}

    wait(NULL);
	return 0;

}