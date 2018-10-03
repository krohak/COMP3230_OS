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
	int children_count = 0;
	
	scanf("%d",&num_children);
	scanf("%d",&num_tasks);
	scanf("%d",&task_per_child);
	
	checkInputs(&num_children, &num_tasks, &task_per_child);
	printf("Start distributing %d task(s) to %d child process(es)\n", num_tasks, num_children);

	pid_t pid;
	// printf("whatttt %d\n", pid);
	
	while(children_count< num_children){

		pid = fork();

		if(pid  < 0){
			return -1;
		}

		else if (pid == 0) // child
		{	
			
			printf("CHILD %d\n", pid);
			exit(0);
		}
		else
		{	
			wait(NULL);
			children_count+=1;
			printf("PARENT %d\n", pid); 
		}

		
		
	}

	return 0;

}