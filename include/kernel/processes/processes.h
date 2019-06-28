//RKS includes
#include "process_types.h"

//============== CREATION / REMOVAL ==============
action_process* create_process(PROCESS_ID, PROCESS_PRIORITY);

int add_process(process_macro*, PROCESS_PRIORITY, char*);
int remove_process(PROCESS_ID);

int add_trigger(process_macro*, trigger_function, PROCESS_ID parent, PROCESS_ID child);
int remove_trigger(process_macro*, PROCESS_ID, OBJECTIVE_ID);

int add_action(process_macro*, action_pool, PROCESS_ID parent, PROCESS_ID child);
int remove_action(process_macro*, PROCESS_ID, OBJECTIVE_ID);


//============== Routing ==============
int route_passive_to(process_macro *, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
int route_active_to(process_macro *, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);

int route_read_ao(process_macro *, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
int route_write_ao(process_macro *, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
