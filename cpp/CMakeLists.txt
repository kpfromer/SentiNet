project(cpp)

# models is a protoc library
add_subdirectory(models)
# add_subdirectory(third_party)

# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(PROTO_LIB protobuf)
find_library(ZMQ_LIB zmq)
find_library(YAML_CPP yaml-cpp)
find_library(CURL_C curl)

include_directories(include)
include_directories(${wii-controller-c_INCLUDE_DIRS})

# Create a static library for messages
file(GLOB_RECURSE MSGS_SRC "${PROJECT_SOURCE_DIR}/include/proto/*.cc" "${PROJECT_SOURCE_DIR}/src/messages/*.cpp")
message(${MSGS_SRC})
set(MESSAGES_LIB messages)
add_library(${MESSAGES_LIB} STATIC ${MSGS_SRC})
target_link_libraries(${MESSAGES_LIB} ${PTHREAD_LIB} ${PROTO_LIB})

file(GLOB_RECURSE CONTROL_SRC "${PROJECT_SOURCE_DIR}/src/control/*.cpp")
set(CONTROL_LIB control)
add_library(${CONTROL_LIB} STATIC ${CONTROL_SRC})
target_link_libraries(${CONTROL_LIB} ${MESSAGES_LIB} ${ZMQ_LIB} ${PTHREAD_LIB})

# Create a static core library for sentinet core
# file(GLOB_RECURSE CORE_SRC_FILES "${PROJECT_SOURCE_DIR}/src/core/*.cpp")
# set(CORE_LIB core)
# add_library(${CORE_LIB} STATIC ${CORE_SRC_FILES})
# target_link_libraries(${CORE_LIB} ${MESSAGES_LIB} ${ZMQ_LIB} ${PTHREAD_LIB})

# Create an implementation specific



# add_executable(run1 "${SRC_DIR}/impl/examples/main_1.cpp")
# target_link_libraries(run1 ${NETWORKING_LIB})
#
# add_executable(run2 "${SRC_DIR}/impl/examples/main_2.cpp")
# target_link_libraries(run2 ${NETWORKING_LIB})
#
# add_executable(all_in_one "${SRC_DIR}/impl/examples/all_in_one.cpp")
# target_link_libraries(all_in_one ${NETWORKING_LIB})
#
# add_executable(example "${SRC_DIR}/impl/example.cpp")
# target_link_libraries(example ${NETWORKING_LIB})
#
# add_executable(client "${SRC_DIR}/impl/examples/client.cpp")
# target_link_libraries(client ${NETWORKING_LIB})
#
# add_executable(server "${SRC_DIR}/impl/examples/server.cpp")
# target_link_libraries(server ${NETWORKING_LIB})
# # add_executable(subscriber "${SRC_DIR}/impl/subscriber.cpp")
# # target_link_libraries(subscriber ${NETWORKING_LIB} ${BOOST_LOG} ${PTHREAD_LIB})
# #
# # add_executable(publisher "${SRC_DIR}/impl/publisher.cpp")
# # target_link_libraries(publisher ${NETWORKING_LIB} ${PTHREAD_LIB} ${BOOST_LOG})
