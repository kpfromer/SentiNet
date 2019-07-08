#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT 3
#define MAX_QUEUE_SIZE 10

typedef struct {
  task** buff;
  const unsigned int front;
  const unsigned int rear;
  sem_t mutex;
  sem_t slots;
  sem_t items;
} job_queue;

typedef struct {
  unsigned int max;
  sem_t mutex;
  sem_t ready;
  sem_t executor;
  job_queue j_queue;
} ready_queue;

typedef struct {
  uint32_t id;    // A 32 bit id that holds a hole bunch of meta information
  char priority;  // A number between 1 and 255 to determine priority
  void (*ingest)();
  void* next;
} task;

// RECURSIVE DEFINITION
void execute_from_queue_recurs(void* ready_task) {
  if (ready_task) {
    task* ready = (task*)ready_task;
    (*ready->ingest)();
    execute_from_queue(ready->next);
  }
  return;
}

// NONRECURSIVE DEFINITION
void* execute_from_queue_discrete(void* ready_task) {
  task* ready = (task*)ready_task;
  (*ready->ingest)();
  return ready->next;
}

/*
  Example low level
    task * task1 = (task*)malloc(sizeof(task*));
    task1->ingest=&print_it1;
    task * task2 = (task*)malloc(sizeof(task*));
    task2->ingest=&print_it2;
    task1->next = task2;
    task2->next = nullptr;
*/
