

#define NULL ((void*)(0))
#define STACK_ALIGN_SIZE sizeof(unsigned char)

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned long
#define uint64_t unsigned long long
#define int8_t char
#define int16_t short
#define int32_t long
#define int64_t long long
#define POINTER void*

#define PID uint8_t  // 0 - 255
#define PROCESS_STATE uint8_t
#define PROCESS_POINTER POINTER
#define PROCESS_STATE uint8_t

#define NUMBER_PROCESSES 10
#define NUMBER_QUEUE_TYPES 3

// Process meta data
typedef struct {
  const PID id;
  const PROCESS_POINTER pointer;  // points to the remote task to be executed
  PROCESS_STATE state;
} process;

typedef struct {
  const process processes[NUMBER_PROCESSES];
  const queue process_controller[NUMBER_QUEUE_TYPES];
} process_table;

typedef struct {
  uint32_t id;
  uint8_t priority;
  void (*execute)();
  void* next;

} queue;

typedef char PID;
typedef unsigned char PROCESS_STATE;

struct struct {
}

pointer process state process number POINTERgram counter
