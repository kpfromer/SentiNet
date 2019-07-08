cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(This)

set(This_VERSION_MAJOR 1)
set(This_VERSION_MINOR 0)

find_package(yaml-cpp REQUIRED)

include_directories(${YAML_CPP_INCLUDE_DIR})

add_executable(run ConfigurationClient.cpp main.cpp)

target_link_libraries(run ${YAML_CPP_LIBRARIES})
