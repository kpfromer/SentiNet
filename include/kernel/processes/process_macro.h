#include "kernel/processes/types.h"
#include "kernel/log/log.h"

int add_action_process(process_macro*, PROCESS_PRIORITY, char*);
int delete_action_process(process_macro*, PROCESS_ID);

int add_action_objective(process_macro*, PROCESS_ID parent, PROCESS_ID child);
int delete_action_objective(process_macro*, PROCESS_ID);
int change_action_objective_name(process_macro*, PROCESS_ID, char*);
int change_action_objective_priority(process_macro*, PROCESS_ID, PROCESS_PRIORITY);
int change_action_objective_pid(process_macro*, PROCESS_ID, PROCESS_ID);

int route_action_objective(process_macro*, PROCESS_ID, PROCESS_ID, PROCESS_ID, PROCESS_ID);
int route_action_objective_write(process_macro*, PROCESS_ID, ACTION_ID, PROCESS_ID);
int route_action_objective_read(process_macro*, PROCESS_ID, ACTION_ID, PROCESS_ID);
