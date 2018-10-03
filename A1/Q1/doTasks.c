#include <stdio.h>
#include <stdlib.h>

void doTasks(int processNum, int begin, int currentNoOfTasksHandled) {
	// processNum: Process Number starts from 1 (Range: 1 - 4)
	// begin: Task Number starts at
	// currentNoOfTasksHandled: The total number of Tasks to be handled by a child process at one time (Range: 1 - 10)
	int j;
	printf("Process %d", (processNum + 1));
	printf("\n");
	for (j = 0; j < currentNoOfTasksHandled; j++) {
		printf("Doing Task %d...", begin++);
		printf("\n");
	}
	printf(
			"------------------------------------------------------------------------------------------------------\n");
} // doTasks

// main Function
int main(int argc, char *argv[]) {

	doTasks(atoi(argv[1]), atoi(argv[2]), atoi(argv[3])); // Cast char * to int by atoi() function

	// return now
	return 0;
} // main
