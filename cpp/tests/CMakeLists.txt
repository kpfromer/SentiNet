project(unit-tests)

set(APP_NAME run_unit_tests)

file(GLOB_RECURSE UNIT_TEST_SOURCE_FILES "*.cpp")
add_executable(${APP_NAME} ${UNIT_TEST_SOURCE_FILES})
add_dependencies(${APP_NAME} gtest)

list(APPEND UNIT_TEST_SUBDIRS "common")
foreach(SUDIR ${UNIT_TEST_SUBDIRS})
  :
