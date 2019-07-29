project(cpp)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(MODELS "${PROJECT_SOURCE_DIR}/models")
add_subdirectory(${MODELS})
include(cpp.cmake)

find_library(PTHREAD_LIB pthread)
find_library(PROTO_LIB protobuf)
find_library(ZMQ_LIB zmq)
find_library(BOOST_LOG boost_log)

add_compile_definitions(DBOOST_LOG_DYN_LINK)

include_directories(${IIRF_CONFIG_DIR} ${INCLUDE_DIR})

file(GLOB_RECURSE PROTO_SRC_DIR "${INCLUDE_DIR}/proto/*.cc")
set(PROTO_MSGS_LIB protomessages)
add_library(${PROTO_MSGS_LIB} SHARED ${PROTO_SRC_DIR})
target_link_libraries(${PROTO_MSGS_LIB} ${PTHREAD_LIB} ${PROTO_LIB} ${BOOST_LOG})

file(GLOB_RECURSE MESSAGE_SRC_DIR "${SRC_DIR}/libs/messages/*.cpp")
set(MESSAGES_LIB messages)
add_library(${MESSAGES_LIB} SHARED ${MESSAGE_SRC_DIR})
target_link_libraries(${MESSAGES_LIB} ${PROTO_MSGS_LIB} ${BOOST_LOG})

file(GLOB_RECURSE NETWORKING_SRC_DIR "${SRC_DIR}/libs/networking/*.cpp")
set(NETWORKING_LIB networking)
add_library(${NETWORKING_LIB} SHARED ${NETWORKING_SRC_DIR})
target_link_libraries(${NETWORKING_LIB} ${MESSAGES_LIB} ${ZMQ_LIB} ${BOOST_LOG} ${PTHREAD_LIB})


add_executable(subscriber "${SRC_DIR}/impl/subscriber.cpp")
target_link_libraries(subscriber ${NETWORKING_LIB} ${BOOST_LOG} ${PTHREAD_LIB})

add_executable(publisher "${SRC_DIR}/impl/publisher.cpp")
target_link_libraries(publisher ${NETWORKING_LIB} ${PTHREAD_LIB} ${BOOST_LOG})
