//RKS includes
#include "kernel/macros.h"

//C includes
#include <stdlib.h>

typedef enum {active, inactive} PROCESS_STATUS;
typedef void (*action_function)(TRIGGER_MESSAGE, void*);
typedef void* action_pool;

//An action objective is a steady stream area that commands an action_process
typedef struct action_objective
{
  PROCESS_ID pid_parent;
  PROCESS_ID pid_child;
  OBJECTIVE_ID oid;
  action_pool data_pool;
  struct action_objective* next;
} action_objective_t;

//A trigger objective is an instantaneous method that triggers an action
typedef struct trigger_objective
{
  PROCESS_ID pid_parent;
  PROCESS_ID pid_child;
  OBJECTIVE_ID oid;
  action_function function;
  struct trigger_objective* next;
} trigger_objective_t;

//An action process is a controller for methods (robot, peripheral, io in the origional architecture)
typedef struct
{
  //CONSTANTS
  PROCESS_ID pid;
  PROCESS_PRIORITY priority;
  PROCESS_STATUS status;

  //TRIGGER OBJECTIVES
  trigger_objective_t passive_to; //ideally only one - handels different 
  trigger_objective_t active_to;

  //ACTION OBJECTIVES
  action_objective_t read_ao;
  action_objective_t write_ao;

  //TRANSITION FUNCTION
  action_function transition_function;
} action_process;


//Heruistic View
typedef struct
{
  verbosity verbosity_level;
  char* process_names[MAXIMUM_PROCESSES];
  action_process* process_table[MAXIMUM_PROCESSES];
  trigger_objective_t* trigger_objective_table[MAXIMUM_OBJECTIVES / 2];
  action_objective_t* action_objective_table[MAXIMUM_OBJECTIVES / 2];
} process_macro;

