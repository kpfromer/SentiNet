#include "kernel/processes/processes.h"

struct robos
{
  process_macro processes;
  rks_stdout output;
  rks_stdin input;
  rks_robot robot;
}

void initialize(struct robos& os);
void initialize_probe(struct robos& os);

void terminate(struct robos& os);
void terminate_probe(struct robos& os);

void start_up_tests(struct robos& os);
void termination_tests(struct robos& os);

void connect_stdout(struct robos& os);
void connect_stdin(struct robos& os);
void connect_robot(struct robos& os);

void stdout_probe(struct robos& os);
void stdin_probe(struct robos& os);
void robot_probe(struct robos& os);
