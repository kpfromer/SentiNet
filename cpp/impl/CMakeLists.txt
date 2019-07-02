project(Kernel-Objective-Based-PubSub)

# Yuk! - change this to make global pool includes
include_directories(include
                    ../common/include)

cmake_minimum_required(VERSION 3.5)

set(SOURCES src/main.cpp)

add_executable(run ${SOURCES})
target_link_libraries(run grl types)
