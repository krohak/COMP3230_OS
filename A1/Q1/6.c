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

int makeTree(int children_count, int child_id, int num_children, int start, int task_per_child, int num_tasks){

	int this_child_id;
	
	if(children_count < num_children){
		
		pid_t pid;
		pid = fork();

		if(pid  < 0){
			return -1;
		}

		else if (pid == 0) // child
		{	
			
			children_count+=1;
			this_child_id = makeTree(children_count, child_id, num_children, start, task_per_child, num_tasks);
			
			if (this_child_id<0){
				return -1;
			}

			this_child_id+=1;
			printf("CHILD %d %d\n", children_count, this_child_id);
			exit(0);
		}
		else
		{	
			wait(NULL);
			printf("PARENT tasks done %d\n", this_child_id); 
			return this_child_id;
		}
	}

	// return 0;

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

	int start = 1;

	for (int i=0; i<= (num_tasks / (task_per_child*num_children)); i++){

		int children_count = 0;
		int child_id = 0;
		makeTree(children_count, child_id, num_children, start, task_per_child, num_tasks);
		// wait(NULL);
		start+=(task_per_child*num_children);
		printf("----------------%d---------------\n",i+1);
	}

	return 0;

}