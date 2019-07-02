/**
 *  @file macros
 *  @brief A BRIEF DESCRIPTION OF THE HEADER FILE
 *  
 *  ADD A MORE DETAILED DESCRIPTION HERE
 *
 *  @author       theo (theo@theo-Lenovo-Yoga-Arch)
 *  @created      Sunday Jun 30, 2019 14:45:44 MDT
 *  @bug No Known Bugs
 */

#ifndef MACROS_HPP
#define MACROS_HPP

//C++ includes
#include <stdint.h>

//RKS includes

//=================================== TRIGGERS ACTIONS AND PROCESSES ==============================

#ifndef TRIGGER_CODE_BYTES
#define TRIGGER_CODE_BYTES 1
#endif  //TRIGGER_CODE_BYTES

#define TRIGGER_CODE_BITS (TRIGGER_CODE_BYTES * 8) //not portable TODO - make work with opperating system
#define TRIGGER_FLAG 1
#define TRIGGER_DEST ((TRIGGER_CODE_BITS / 2) - TRIGGER_FLAG)
#define TRIGGER_DATA (TRIGGER_CODE_BITS / 2)

typedef struct
{
  unsigned char flag:TRIGGER_FLAG;
  unsigned char dest:TRIGGER_DEST;
  unsigned char data:TRIGGER_DATA;
} trigger_message;

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

//============================================ UTILITY ============================================
#define PRINT_TIME(_s1, _tb, _s2) \
  do { \
    printf( "%s%02d:%02d:%02d   %02d/%02d/%04d%s", \
       _s1, (int) (_tb)->hour, (int) (_tb)->minute, (int) (_tb)->second, \
       (int) (_tb)->month, (int) (_tb)->day, (int) (_tb)->year, _s2 ); \
    fflush(stdout); \
  } while ( 0 )



#endif /* end of include guard MACROS_HPP */

