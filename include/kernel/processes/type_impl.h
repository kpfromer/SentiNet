#include "kernel/processes/process_types.h"



#define DELETE( a ) free( (a) )

//Action process

#define SET_STATICS( app )  //TODO
#define SET_STATUS( app )   //TODO
action_process* create_action_process(PROCESS_ID, PROCESS_PRIORITY);
int remove_action_objective_read(action_process*, ACTION_ID);
int remove_action_objective_write(action_process*, ACTION_ID);


trigger_objective_t* create_trigger_objective(action_process*, process*);
int add_process(trigger_objective_t*, process);
//TODO once you define codes - add a "add code" function

action_objective_t* create_action_objective(ACTION_ID, action_pool);
//int write_action_objective(actionvoid*);
//void* read_action_objective(action_objective_t*)
int delete_action_objective
