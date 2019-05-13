/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>

struct tcb{
	  int thread_id;
    int thread_priority;
	  ucontext_t *thread_context;
	  struct tcb *next;
		struct sem_t *s;
};
typedef struct tcb tcb;

struct sem_t{
	int count;
	struct tcb *q;
};
typedef struct sem_t sem_t;
