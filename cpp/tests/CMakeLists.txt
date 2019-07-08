project(impl)

set(IIRF_IMPL_INCLUDES include)

include_directories(../framework/include
                    ${IIRF_IMPL_INCLUDES})


add_executable(curl_test run-tests/CurlTest.cpp)
target_link_libraries(curl_test curlloader)
set(CLIENT run-tests/Clienttest.cpp)
set(SERVER run-tests/Servertest.cpp)
add_executable(client ${CLIENT})
add_executable(server ${SERVER})
target_link_libraries(client zmqclient)
target_link_libraries(server zmqserver)
