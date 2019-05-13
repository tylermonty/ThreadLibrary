/*
 * thread library function prototypes
 */

 #include "t_lib.h"

void t_init(void);
void t_create(void(*function)(int), int thread_id, int priority);
void t_yield(void);
void t_shutdown();
void t_terminate();
tcb *enqueue(tcb **queue, tcb *node);
tcb *dequeue(tcb **queue);
int sem_init(sem_t **sp, int sem_count);
void sem_wait(sem_t *sp);
void sem_signal(sem_t *sp);
void sem_destroy(sem_t **sp);
