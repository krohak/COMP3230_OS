#include <stdio.h>

int factorialNum(int num);

int main() {
	int num = 10;

	printf("\nThe %d! is: %d\n", num, factorialNum(num));

	return 0;
} // main

int factorialNum(int num) {
	static int numOfCall = 0;
	numOfCall++;

	printf("factorialNum is executed for %d time(s).\n", numOfCall);

	if (num >= 1) {
		return num * factorialNum(num - 1);
	} else {
		return 1;
	}
} // factorialNum
