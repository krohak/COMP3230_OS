DEBUG = 0
CC = gcc
CFLAGS = -Wall -fopenmp -pthread 
ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += #-O2
endif

proCon.out: thread_func.h thread_func.c proCon.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm *.out
