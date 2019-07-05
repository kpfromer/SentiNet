project(Kernelo-Objective-Based-PubSub)

include_directories(include)

cmake_minimum_required(VERSION 3.5)

set(LIBZMQ_CLIENT src/ZMQClient.cpp
					src/ClientInterface.cpp)
set(LIBZMQ_SERV src/ZMQServer.cpp
					src/ServerInterface.cpp)
		  	

add_library(zmqclient ${LIBZMQ_CLIENT})
add_library(zmqserver ${LIBZMQ_SERV})

find_library(LIB_ZMQ zmq)
target_link_libraries(zmqclient ${LIB_ZMQ})
target_link_libraries(zmqserver ${LIB_ZMQ} pthread)
