#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {

    printf("COMP3230B Principles of Operating Systems\t Assignment 1\t SINGHAL Rohak 3035242475\n");

    pid_t pid;

    if((pid = fork()) < 0)
		return -1;

	if (pid == 0) // child
	{
		printf("CHILD %d\n", pid);
        exit(0);
	}
	else
	{
		
        wait(NULL);
		printf("PARENT %d\n", pid); 
	}

    // wait(NULL);
	return 0;

}