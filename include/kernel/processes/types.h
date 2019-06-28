#include "../macros.h"

#include <stdlib.h>
#include <pthread.h>


//============================ Forward declaration ============================
//Large Scale Structures
struct action_process;
struct trigger_objective;
struct action_objective;

//Small scale types
typedef enum {active, inactive} PROCESS_STATE;

//Processes and pools
typedef void* action_pool;
typedef void* (*process)(struct action_process*, TRIGGER_MESSAGE)
typedef void (*trigger_function)(TRIGGER_MESSAGE);


//=============================== Definitions =================================
typedef struct
{
  struct action_process* ap;  //Only to pass as a trigger message
  process * processes;        //array of processes
  trigger_function trigger;   //execute a process from process array
} trigger_objective_t;        

typedef struct
{
  ACTION_ID aid;              //For being able to reference and for loggin purposes
  action_pool data;           //Abstract data (most likely a struct)
} action_objective_t;

//TODO make typedefed
struct action_process
{
  //statics
  PROCESS_ID pid;             //Logging and heuristic control
  PROCESS_PRIORITY priority;  //Logging and heurusitic control
  PROCESS_STATUS status;      //Loggin and heuristici control
  pthread_mutex_t mutex;      //to avoid race conditions in read write processes

  //dynamics
  trigger_objective_t* trigger;       //has a trigger - calls 
  action_objective_t** read_actions;  //read only
  action_objective_t** write_actions; //write only
  trigger_function trigger;           //same trigger function as it's trigger_objective_t
}

//A map of memory
typedef struct
{
  verbosity verbosity_level;
  char* process_names[MAXIMUM_PROCESSES];
  struct action_process* action_process_table[MAXIMUM_ACTION_PROCESSES];
  action_objective_t * [MAXIMUM_OBJECTIVES];
} process_macro;




