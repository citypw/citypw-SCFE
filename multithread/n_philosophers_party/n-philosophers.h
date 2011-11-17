#ifndef __N_PHILOSOPHERS_H
#define __N_PHILOSOPHERS_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 3 status of each philosopher */
#define THINKING 0
#define HUNGRY 1
#define EATING 2

/* how much numbers of philosophers */
#define NR 70

int phv_index[NR] = {0};

pthread_mutex_t cp_mutex[2] = { PTHREAD_MUTEX_INITIALIZER };

/* chopsticks are shared object */
unsigned cs_shared[NR] = { 0 };

unsigned count = 0;

struct philosopher {
	char name[20];
	unsigned short hands[2];
	unsigned short status;
	unsigned int ith;
	pthread_t tid;
};

#endif
