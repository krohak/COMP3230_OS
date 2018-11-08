DEBUG = 1
CC = gcc
CFLAGS = -fopenmp -pthread -Wall 

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += #-O3
endif

hello.o: hello.c
	$(CC) $(CFLAGS) $? -o $@

cal_sum.o: cal_sum.c
	$(CC) $(CFLAGS) $? -o $@

pi_answer.o: pi_answer.c
	$(CC) $(CFLAGS) $? -o $@

fix_counter.o: fix_counter.c
	$(CC) $(CFLAGS) $? -o $@

fix_counter_wrong_answer.o: fix_counter_wrong_answer.c
	$(CC) $(CFLAGS) $? -o $@

pi.o: pi.c
	$(CC) $(CFLAGS) $? -o $@

semaphore_binary.o: semaphore_binary.c
	$(CC) $(CFLAGS) $? -o $@

semaphore.o: semaphore.c
	$(CC) $(CFLAGS) $? -o $@

clean:
	rm -f hello cal_sum fix_counter fix_counter_wrong_answer pi semaphore semaphore_binary
