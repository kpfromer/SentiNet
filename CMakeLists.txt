#Set version of cmake verison
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(RobotKernelSystems VERSION 0.0.1 LANGUAGES C;CXX DESCRIPTION "Robotic kernel system")
set(RKS_CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/cpp)

if(NOT DATE)
  string(TIMESTAMP DATE "%Y%m%d-%H%M%S")
endif()

set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_APPDIR "app")
set(CMAKE_INSTALL_TESTDIR "test")


#C++14
set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES src/*.cpp)
include_directories(include)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/rks.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE .)

#target_include_directories(${PROJECT_NAME} PRIVATE include)

include(GNUInstallDirs)


install(TARGETS ${PROJECT_NAME} 
  LIBRARY DESTINATION lib/${PROJECT_NAME}
  PUBLIC_HEADER DESTINATION include)
