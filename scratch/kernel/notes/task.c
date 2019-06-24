#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  int data;
  void (*func)(int);
  void* next;
} process_task;

void * ingest(void* val)
{
  process_task* value = (process_task*)val;

  printf("internal data: %d\n", value->data);

  (*(value->func))(value->data);

  return value->next;
}


void print(int a){
  printf("internal function pointer %d\n", a);
}

int main(void)
{
  process_task * a = malloc(sizeof(process_task));
  a->data = 1;
  a->func = print;
  
  process_task * b = malloc(sizeof(process_task));
  b->data = 2;
  b->func = print;

  a->next = b;
  b->next = NULL;

  ingest(a);
  ingest(b);

  free(a);
  free(b);
  return 1;
}
