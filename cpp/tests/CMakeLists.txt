project(Kernel-Objective-Based-PubSub)

include_directories(../common/include)

add_executable(client unit-tests/Clienttest.cpp)
add_executable(server unit-tests/Servertest.cpp)

target_link_libraries(client zmqclient)
target_link_libraries(server zmqserver)
