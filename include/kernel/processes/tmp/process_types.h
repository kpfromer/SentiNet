//RKS includes
#include "../macros.h"

//C includes
#include <stdlib.h>

//Forward declaration
struct action_process;
struct trigger_objective;
struct action_objective;
enum PROCESS_STATUS;

//A Process can be utilized or not, but if its active, it is either waiting
//for one of its functions to complete or executing a function
typedef enum {active, inactive} PROCESS_STATUS;

//A trigger function. Recieves a number (TRIGGER_MESSAGE) and a pointer to an action process
typedef void (*trigger_function)(TRIGGER_MESSAGE, struct action_process*);

//An arbitrary pool of data (templated struct without c++ template)
typedef void* action_pool;

//An action objective is a steady stream area that commands an action_process
typedef struct action_objective
{
  OBJECTIVE_ID oid;
  action_pool data_pool;
  struct action_objective* next;
} action_objective_t;

//A trigger objective is an instantaneous method that triggers an action
typedef struct trigger_objective
{
  OBJECTIVE_ID oid;
  trigger_function process;
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
  trigger_objective_t* triggers;

  //ACTION OBJECTIVES
  action_objective_t* actions;

  //TRANSITION FUNCTION
  trigger_function transition_function;
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

