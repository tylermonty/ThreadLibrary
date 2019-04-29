/*
 * thread library function prototypes
 */

 #include "t_lib.h"

void t_init(void);
void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_shutdown();
void t_terminate();
void enqueue(t_queue *queue, tcb *node);
tcb *dequeue(t_queue *queue);
void printList(t_queue *queue);
