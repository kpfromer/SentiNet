# Set the project name
project(unit-tests)

# Include the top level path for #include resolution
include_directories("${KELVIN_CPP_SOURCE_DIR}/tests/include" "${KELVIN_CPP_SOURCE_DIR}/applications")
include_directories(SYSTEM "${PREBUILT_INSTALL_INCDIR}/zmq")

set(APP_NAME run_unit_tests)

# Add all of the unit test files into a single unit test application
file(GLOB_RECURSE UNIT_TEST_SOURCE_FILES "*.cpp")

set(ALL_KELVIN_DEPENDENCIES ${DATA_MODEL} application crc)

# Create the unit test executable
add_executable(${APP_NAME} ${UNIT_TEST_SOURCE_FILES})
# Build the unit test libraries
add_dependencies(${APP_NAME} gtest)
# Why is the gtest libaray path such a pain in the ass ... ugh
target_link_libraries(${APP_NAME} "${binary_dir}/googlemock/gtest/libgtest.a" ${ALL_KELVIN_DEPENDENCIES} util stdc++fs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")
set_property(TARGET ${APP_NAME} PROPERTY INSTALL_RPATH "${CMAKE_INSTALL_RPATH}")

#########
# We are going to make use there is at least a unit test that inflates our DSOs with a compiler check.
# Currently the only way to make sure that DSOs are being loaded properly is do a runtime check, this
# will check a compile/link time check automatically.
list(APPEND UNIT_TEST_SUBDIRS "applications" "protocols")
foreach(SUBDIR ${UNIT_TEST_SUBDIRS})
  ## Check to see if there is a new application without a unit test
  subdirlist(APP_NAMES "${KELVIN_CPP_SOURCE_DIR}/${SUBDIR}")
  subdirlist(UNIT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
  ## Remove GPSD until ticket KEMB-315 is addressed
  list(REMOVE_ITEM APP_NAMES GPSD)
  foreach(UNIT_TEST ${UNIT_TESTS})
    list(REMOVE_ITEM APP_NAMES "${UNIT_TEST}")
  endforeach()

  ## Generate a unit test for all of the untested applications
  foreach(APP ${APP_NAMES})
    set(GEN_UNIT_TEST "generate_${SUBDIR}_${APP}_unit_test")
    add_custom_target(${GEN_UNIT_TEST} ALL)
    add_custom_command(TARGET ${GEN_UNIT_TEST}
      COMMENT "Auto-generate ${SUBDIR}/${APP} unit test for new applications"
      COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/${APP}
      COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/MinimumTest.sample ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/${APP}/${APP}.cpp
      COMMAND sed -i 's/{{ TYPE }}/${SUBDIR}/g' ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/${APP}/${APP}.cpp
      COMMAND sed -i 's/{{ NAME }}/${APP}/g' ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/${APP}/${APP}.cpp)
  endforeach()
endforeach()

## Add the generated unit tests to the build system
foreach(SUBDIR ${UNIT_TEST_SUBDIRS})
  ## Check to see if there is a new application without a unit test
  subdirlist(UNIT_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
  ## Remove GPSD until ticket KEMB-315 is addressed
  list(REMOVE_ITEM UNIT_TESTS GPSD)
  foreach(TEST ${UNIT_TESTS})
    target_link_libraries(${APP_NAME} ${TEST})
    list(APPEND ALL_KELVIN_DEPENDENCIES ${TEST})

    # Add the sample data directory if it is present
    set(SAMPLE_DATA "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/${TEST}/sample_data")
    if(EXISTS "${SAMPLE_DATA}")
      add_custom_command(TARGET ${APP_NAME}
        COMMENT "Loading sample data"
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${TEST}
        COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_TESTDIR}/${TEST}
        COMMAND cp -r ${SAMPLE_DATA} ${CMAKE_CURRENT_BINARY_DIR}/${TEST}
        COMMAND cp -r ${SAMPLE_DATA} ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_TESTDIR}/${TEST})
    endif()
  endforeach()
endforeach()

# Register the test with CMake
if(${USE_CODE_COVERAGE})
  SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage EXECUTABLE ./${APP_NAME} --gtest_shuffle DEPENDENCIES ${ALL_KELVIN_DEPENDENCIES})
endif()

# Set what artifacts are included in the installation target
install(TARGETS ${APP_NAME}
  EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_TESTDIR})

# Since the code coverage executes in the cmake-artfiacts directory, we need to create symlinks so that we don't have to change the unit test code.
add_custom_command(TARGET ${APP_NAME}
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_APPDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_PROTOCOLDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_MSGDIR} ${CMAKE_CURRENT_BINARY_DIR}/../
  COMMAND ln -fs ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CFGDIR} ${CMAKE_CURRENT_BINARY_DIR}/../)

