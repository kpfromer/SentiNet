//C includes
#include <stdint.h>

#ifndef ACTION_NAME_SIZE
#define ACTION_NAME_SIZE 10
#endif  //ACTION_NAME_SIZE

#ifndef MAXIMUM_PROCESSES
#define MAXIMUM_PROCESSES 20
#endif  //MAXIMUM_PROCESSES

#ifndef MAXIMUM_OBJECTIVES
#define MAXIMUM_OBJECTIVES 40
#endif  //MAXIMUM_OBJECTIVES

#define PROCESS_ID        uint32_t
#define OBJECTIVE_ID      uint32_t
#define PROCESS_PRIORITY  uint8_t
#define TRIGGER_MESSAGE   uint8_t

typedef enum {none, low, medium, high} verbosity;

