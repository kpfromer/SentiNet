cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)

project(obps)

add_executable(run
	TriggerHandler.cpp
	ActionProcess.cpp
	Types.cpp
	main.cpp)
