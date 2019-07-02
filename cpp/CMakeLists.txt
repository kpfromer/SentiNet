project(Kernel-Objective-Based-PubSub)


include_directories(common)
add_subdirectory(lib)

add_executable(run impl/main.cpp
	src/ActionProcess.cpp
	src/TriggerHandler.cpp)

target_link_libraries(run types)
