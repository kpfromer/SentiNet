project(impl)

set(IIRF_IMPL_INCLUDES include)

include_directories(../common/include
                    ${IIRF_IMPL_INCLUDES})

set(CLIENT unit-tests/Clienttest.cpp)
set(SERVER unit-tests/Servertest.cpp)

add_executable(client ${CLIENT})
add_executable(server ${SERVER})

target_link_libraries(client zmqclient)
target_link_libraries(server zmqserver)
