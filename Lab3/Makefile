DEBUG = 1
CC = gcc
CFLAGS = -fopenmp -pthread -Wall 

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += #-O3
endif

hello: hello.c
	$(CC) $(CFLAGS) $? -o $@

cal_sum: cal_sum.c
	$(CC) $(CFLAGS) $? -o $@

cal_pi: cal_pi.c
	$(CC) $(CFLAGS) $? -o $@

fix_counter: fix_counter.c
	$(CC) $(CFLAGS) $? -o $@

fix_counter_wrong_answer: fix_counter_wrong_answer.c
	$(CC) $(CFLAGS) $? -o $@

pi: pi.c
	$(CC) $(CFLAGS) $? -o $@

semaphore_binary: semaphore_binary.c
	$(CC) $(CFLAGS) $? -o $@

semaphore: semaphore.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -f hello cal_sum fix_counter fix_counter_wrong_answer pi semaphore semaphore_binary
