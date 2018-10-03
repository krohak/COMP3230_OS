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

void makeTree(int children_count, int num_children, int start, int task_per_child, int num_tasks){

	if(children_count < num_children){
		
		pid_t pid;
		pid = fork();

		if(pid  < 0){
			return;
		}

		else if (pid == 0) // child
		{	
			children_count+=1;
			start-=task_per_child;
			makeTree(children_count, num_children, start, task_per_child, num_tasks);
			// printf("CHILD %d and processes %d\n", (num_children-children_count+1), start+1);

			char process_number [10], process_start [10], process_delta [10] ;
			
			snprintf ( process_number, 10, "%d",  (num_children-children_count));
			snprintf ( process_start, 10, "%d",  start+1);
			snprintf ( process_delta, 10, "%d",  task_per_child);
			
			if (start < num_tasks-task_per_child ){
				execlp("./doTasks", "./doTasks", process_number,  process_start, process_delta, (char*) 0 );
			}
			else if (start >= num_tasks-task_per_child && start < num_tasks) {
				snprintf ( process_delta, 10, "%d",  num_tasks-start);
				execlp("./doTasks", "./doTasks", process_number,  process_start, process_delta, (char*) 0 );
			}	
			exit(0);
		}
		else
		{	
			int status;
			wait(&status);
			// printf("PARENT tasks done and finished %d processes\n", start); 
			return;
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

	int start = 0;

	for (int i=0; i<= (num_tasks / (task_per_child*num_children)); i++){

		int children_count = 0;
		start+=(task_per_child*num_children);
		if (start > num_tasks+(task_per_child*num_children) && i>0){
			break;
		}
		makeTree(children_count, num_children, start, task_per_child, num_tasks);
		// wait(NULL);
		printf("|||||||||||||------%d-----|||||||||||||\n",i+1);
	}

	return 0;

}