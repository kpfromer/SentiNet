# Include the GTEST CMake configuration file.  I didn't want to pollute the main file with GTEST stuff
include(CMakeLists-gtest.txt)

# Include the unit tests (should pass on all machines)
add_subdirectory(unit_test)
# Include the functional tests (may pass on a machine, but passing is based on performance (which can vary per machine))
add_subdirectory(functional_test)
