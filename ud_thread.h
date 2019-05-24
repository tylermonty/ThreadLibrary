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

//mailbox functions
int mbox_create(mbox **mb);
void mbox_destroy(mbox **mb);
void mbox_deposit(mbox *mb, char *msg, int len);
void mbox_withdraw(mbox *mb, char *msg, int *len);
messageNode *enq(messageNode **queue, messageNode *node);
messageNode *deq(messageNode **queue);
void deposit_helper(mbox* mb, messageNode* node);
//message functions
void send(int tid, char *msg, int len);
void receive(int *tid, char *msg, int *length);
void block_send(int tid, char *msg, int len);
void block_receive(int *tid, char *msg, int *length);
