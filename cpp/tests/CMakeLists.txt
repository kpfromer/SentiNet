project(Kernel-Objective-Based-PubSub)

include_directories(include
					../common/include)

add_executable(objective-handler-test unit-tests/ObjectiveHandlerTest.cpp)
add_executable(objective-test unit-tests/ActionObjectiveTest.cpp)
add_executable(action-trigger-test unit-tests/ActionTriggerTest.cpp)

target_link_libraries(action-trigger-test grl types)
target_link_libraries(objective-handler-test grl types)
target_link_libraries(objective-test grl types)
