cmake_minimum_required(VERSION 3.5)

project(thing)




find_library(YAML yaml-cpp)
add_library(utils SHARED utils.cpp)
target_link_libraries(utils)

add_executable(run main.cpp)


target_link_libraries(run ${utils} ${YAML} ${PTHREAD} ${BOOST_LOG})


