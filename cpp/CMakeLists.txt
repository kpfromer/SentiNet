project(cpp)

# Set global stuff
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(MODELS "${PROJECT_SOURCE_DIR}/models")
set(THIRD_PARTY "${PROJECT_SOURCE_DIR}/third_party")

# models is a protoc library
add_subdirectory(${MODELS})
add_subdirectory(${THIRD_PARTY})


# Find the necessary libraries
find_library(PTHREAD_LIB pthread)
find_library(PROTO_LIB protobuf)
find_library(ZMQ_LIB zmq)
# find_library(BOOST_LOG boost_log)
find_library(YAML_CPP yaml-cpp)
find_library(CURL_C curl)

# add_compile_definitions(DBOOST_LOG_DYN_LINK)

include_directories(${INCLUDE_DIR})

# Protobuf defined messages
file(GLOB_RECURSE PROTO_SRC_DIR "${INCLUDE_DIR}/proto/*.cc")
set(PROTO_MSGS_LIB protomessages)
add_library(${PROTO_MSGS_LIB} STATIC ${PROTO_SRC_DIR})
target_link_libraries(${PROTO_MSGS_LIB} ${PTHREAD_LIB} ${PROTO_LIB} ${BOOST_LOG})
install(TARGETS ${PROTO_MSGS_LIB} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)


file(GLOB_RECURSE MESSAGE_SRC_DIR "${SRC_DIR}/libs/messages/*.cpp")
set(MESSAGES_LIB messages)
add_library(${MESSAGES_LIB} STATIC ${MESSAGE_SRC_DIR})
target_link_libraries(${MESSAGES_LIB} ${PROTO_MSGS_LIB} ${BOOST_LOG})
install(TARGETS ${MESSAGES_LIB} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

file(GLOB_RECURSE NETWORKING_SRC_DIR "${SRC_DIR}/libs/networking/*.cpp")
set(NETWORKING_LIB networking)
add_library(${NETWORKING_LIB} STATIC ${NETWORKING_SRC_DIR})
target_link_libraries(${NETWORKING_LIB} ${MESSAGES_LIB} ${ZMQ_LIB} ${BOOST_LOG} ${PTHREAD_LIB} ${YAML_CPP})
install(TARGETS ${NETWORKING_LIB} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

file(GLOB_RECURSE CURLCPP_SRCS "${SRC_DIR}/libs/curlcpp/*.cpp")
set(CURLCPP_LIB curlcpp)
add_library(${CURLCPP_LIB} STATIC ${CURLCPP_SRCS})
target_link_libraries(${CURLCPP_LIB} ${CURL_C})
install(TARGETS ${CURLCPP_LIBS} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)




# Executables
add_executable(run1 "${SRC_DIR}/impl/examples/main_1.cpp")
target_link_libraries(run1 ${NETWORKING_LIB})

add_executable(run2 "${SRC_DIR}/impl/examples/main_2.cpp")
target_link_libraries(run2 ${NETWORKING_LIB})

add_executable(all_in_one "${SRC_DIR}/impl/examples/all_in_one.cpp")
target_link_libraries(all_in_one ${NETWORKING_LIB})

add_executable(example "${SRC_DIR}/impl/example.cpp")
target_link_libraries(example ${NETWORKING_LIB})
# add_executable(subscriber "${SRC_DIR}/impl/subscriber.cpp")
# target_link_libraries(subscriber ${NETWORKING_LIB} ${BOOST_LOG} ${PTHREAD_LIB})
#
# add_executable(publisher "${SRC_DIR}/impl/publisher.cpp")
# target_link_libraries(publisher ${NETWORKING_LIB} ${PTHREAD_LIB} ${BOOST_LOG})
