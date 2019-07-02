project(Kernelo-Objective-Based-PubSub)

include_directories(include)

cmake_minimum_required(VERSION 3.5)

set(LIBTYPES_SRC src/Types.cpp)
set(LIBGRL_SRC src/TriggerHandler.cpp
              src/ActionProcess.cpp)


add_library(types ${LIBTYPES_SRC})
add_library(grl ${LIBGRL_SRC})

target_link_libraries(grl types)
