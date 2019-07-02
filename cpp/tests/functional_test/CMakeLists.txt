# Set the project name
project(functional-tests)

# Include the top level path for #include resolution
include_directories("${KELVIN_CPP_SOURCE_DIR}/tests/include" "${KELVIN_CPP_SOURCE_DIR}/applications")

set(APP_NAME run_functional_tests)

# Add all of the unit test files into a single unit test application
file(GLOB_RECURSE UNIT_TEST_SOURCE_FILES *.cpp)

# Create the unit test executable
add_executable(${APP_NAME} ${UNIT_TEST_SOURCE_FILES})
# Build the unit test libraries
add_dependencies(${APP_NAME} gtest)
# Why is the gtest libaray path such a pain in the ass ... ugh
target_link_libraries(${APP_NAME} "${binary_dir}/googlemock/gtest/libgtest.a" ${DATA_MODEL} application kelvinclient crc stdc++fs)
set_property(TARGET ${APP_NAME} PROPERTY INSTALL_RPATH "${CMAKE_INSTALL_RPATH}")

# Set what artifacts are included in the installation target
install(TARGETS ${APP_NAME}
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_TESTDIR})

