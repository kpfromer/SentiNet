#include "processes.h"

action_process* create_process(PROCESS_ID pid, PROCESS_PRIORITY pp)
{
  action_process* process = malloc(sizeof(action_process));
  process->pid = pid;
  process->priority = pp;
  process->status = inactive;
  process->triggers = NULL;
  process->actions = NULL;
  process->transition_function = NULL;
}

trigger_objective_t create_trigger_objective(void (*a)(...))

void printdatbooty(int i, int j)
{
  printf("booty\n");
}

int main()
{
  trigger_objective_t* toa = create_trigger_objective(printdatbooty)
  action_process* a = create_process(10, 10);
  return 0;
}
