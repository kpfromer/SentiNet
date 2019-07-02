cmake_minimum_required(VERSION 3.5)

project(Kernel-Objective-Based-PubSub VERSION 0.0.1 LANGUAGES C;CXX)
set(CPP_SOURCE_DIR ${PROJECT_SOURCE_DIR}/cpp)

set(CMAKE_CXX_STANDARD 14)

if(NOT DATE)
	string(TIMESTAMP DATE "%Y%m%d-%H%M%S")
endif()

set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")

add_subdirectory(${CPP_SOURCE_DIR})


