project(cpp)

# Set global stuff
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(MODELS "${PROJECT_SOURCE_DIR}/models")

# models is a protoc library
add_subdirectory(${MODELS})

# C++ options
include(cpp.cmake)

# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(PROTO_LIB protobuf)
find_library(ZMQ_LIB zmq)
# find_library(BOOST_LOG boost_log)
find_library(YAML_CPP yaml-cpp)

# add_compile_definitions(DBOOST_LOG_DYN_LINK)

include_directories(${INCLUDE_DIR})

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

add_executable(run "${SRC_DIR}/impl/main.cpp")
target_link_libraries(run ${NETWORKING_LIB} ${PTHREAD_LIB} ${YAML_CPP})

add_executable(run2 "${SRC_DIR}/impl/main2.cpp")
target_link_libraries(run2 ${NETWORKING_LIB} ${PTHREAD_LIB} ${YAML_CPP})

add_executable(all_in_one "${SRC_DIR}/impl/all_in_one.cpp")
target_link_libraries(all_in_one ${NETWORKING_LIB} ${PTHREAD_LIB} ${YAML_CPP})

add_executable(example "${SRC_DIR}/impl/example.cpp")
target_link_libraries(example ${NETWORKING_LIB} ${PTHREAD_LIB} ${YAML_CPP})
# add_executable(subscriber "${SRC_DIR}/impl/subscriber.cpp")
# target_link_libraries(subscriber ${NETWORKING_LIB} ${BOOST_LOG} ${PTHREAD_LIB})
#
# add_executable(publisher "${SRC_DIR}/impl/publisher.cpp")
# target_link_libraries(publisher ${NETWORKING_LIB} ${PTHREAD_LIB} ${BOOST_LOG})
