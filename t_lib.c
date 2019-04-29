//#include "t_lib.h"
#include "ud_thread.h"

tcb *running;
t_queue *ready;

void t_init() //initialize thread library
{
  tcb *tmp;
  tmp = malloc(sizeof(tcb));
  tmp->thread_id = 0;
  tmp->thread_priority = 1;
  tmp->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  tmp->next = NULL;
  getcontext(tmp->thread_context);    /* let tmp be the context of main() */
  running = tmp;
  ready = (t_queue *)calloc(1, sizeof(t_queue));
  ready->head = NULL;
}

void t_create(void (*fct)(int), int id, int pri) //create new thread
{
  size_t sz = 0x10000;

  tcb *new_tcb = malloc(sizeof(tcb));
  new_tcb->thread_context = (ucontext_t *) malloc(sizeof(ucontext_t));
  getcontext(new_tcb->thread_context);
/***
  uc->uc_stack.ss_sp = mmap(0, sz,
       PROT_READ | PROT_WRITE | PROT_EXEC,
       MAP_PRIVATE | MAP_ANON, -1, 0);
***/
  new_tcb->thread_context->uc_stack.ss_sp = malloc(sz);
  new_tcb->thread_context->uc_stack.ss_size = sz;
  new_tcb->thread_context->uc_stack.ss_flags = 0;
  makecontext(new_tcb->thread_context, (void (*)(void)) fct, 1, id);
  new_tcb->thread_id = id;
  new_tcb->thread_priority = pri;
  new_tcb->next = NULL;
  enqueue(ready, new_tcb);
}

void t_yield() //thread relinquishes CPU, placed at end of ready queue
{
  if (ready->head){ //only yield if ready queue has at least one node
    tcb *new_running = dequeue(ready); //get node with highest priority
    tcb *old_running = running;
    new_running->next = NULL;
    old_running->next = NULL;
    enqueue(ready, old_running); //place thread back onto queue
    running = new_running;
    swapcontext(old_running->thread_context, new_running->thread_context);
  }
}

void t_shutdown(){ //shutdown thread library by freeing all allocated memory
  if (ready->head){
    tcb *cur = ready->head;
    while(cur->next){
      tcb *tmp = cur;
      cur = cur->next;
      free(tmp->thread_context);
      free(tmp);
    }
    free(cur->thread_context);
    free(cur);
  }
  free(ready);
  free(running);
}

void t_terminate(){ //terminate calling thread
  tcb *tmp = running;
  free(tmp->thread_context);
  free(tmp);
  running = dequeue(ready);
  if (running){
    setcontext(running->thread_context);
  }
}

void enqueue(t_queue *queue, tcb *node){
  if (!queue->head){ //if queue is empty, insert as first element
    queue->head = node;
    node->next = NULL;
  }
  else{ //find where to insert node based on priority
    if (queue->head->thread_priority > node->thread_priority){ //head of queue
      node->next = queue->head;
      queue->head = node;
    }
    tcb *cur = queue->head;
    while (cur->next && cur->next->thread_priority <= node->thread_priority){
      cur = cur->next;
    }
    node->next = NULL;
    if (cur->next){ //if not at end of list, point to next element
      node->next = cur->next->next;
    }
    cur->next = node;
  }
}

tcb *dequeue(t_queue * queue){ //dequeue first node in queue
  tcb *tmp;
  if (queue){
    tmp = queue->head;
    queue->head = tmp->next;
    tmp->next = NULL;
  }
  tmp->next = NULL;
  return tmp;
}

void printList(t_queue *queue){ //print thread library list
  printf("Thread currently runing: %d\n", running->thread_id);
  if (queue){ //check if queue is empty before printing
    tcb *cur = queue->head;
    while(cur){
      printf("%d\n", cur->thread_id);
      cur = cur->next;
    }
    printf("\n");
  }
  else{
    printf("queue is empty\n");
  }
}
