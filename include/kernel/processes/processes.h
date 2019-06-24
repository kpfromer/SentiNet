//RKS includes
#include "kernel/processes/processes.h"

//============== CREATION / REMOVAL ==============
//Add or remove an action process
void add_process(process_macro&, PROCESS_PRIORITY, char*);
void add_process(process_macro&, action_process);
void add_process(process_macro&, char*);

void remove_process(process_macro&, PROCESS_PRIORITY, char*);
void remove_process(process_macro&, action_process);
void remove_process(process_macro&, char*);

//Add or remove a passive trigger objective
void add_passive_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, trigger_objective_t);
void add_passive_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_function);
//unassigned
void add_passive_to(process_macro&, trigger_objective_t);
void add_passive_to(process_macro&, action_function);

void remove_passive_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, trigger_objective_t);
void remove_passive_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_function);
void remove_passive_to(process_macro&, OBJECTIVE_ID); //Most common
//unassigned
void remove_passive_to(process_macro&, trigger_objective_t);
void remove_passive_to(process_macro&, action_function);
void remove_passive_to(process_macro&, OBJECTIVE_ID);

//Add or remove an active trigger objective
void add_active_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, trigger_objective_t);
void add_active_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_function);
//Unassigned
void add_active_to(process_macro&, trigger_objective_t);
void add_active_to(process_macro&, action_function);

void remove_active_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, trigger_objective_t);
void remove_active_to(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_function);
void remove_active_to(process_macro&, OBJECTIVE_ID); //Most common
//Unassigned
void remove_active_to(process_macro&, trigger_objective_t);
void remove_active_to(process_macro&, action_function);
void remove_active_to(process_macro&, OBJECTIVE_ID);

//Add or remove a read only action objective
void add_read_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_objective_t);
void add_read_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_pool);

void remove_read_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_objective_t);
void remove_read_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_pool);
void remove_active_to(process_macro&, OBJECTIVE_ID); //Most common

//Add or remove a write only action objective
void add_write_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_objective_t);
void add_write_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_pool);

void remove_write_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_objective_t);
void remove_write_ao(process_macro&, PROCESS_ID parent, PROCESS_ID child, action_pool);
void remove_write_ao(process_macro&, OBJECTIVE_ID);



//============== Routing ==============
void route_passive_to(process_macro&, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
void route_active_to(process_macro&, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);

void route_read_ao(process_macro&, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
void route_write_ao(process_macro&, OBJECTIVE_ID old_oid, OBJECTIVE_ID new_oid);
