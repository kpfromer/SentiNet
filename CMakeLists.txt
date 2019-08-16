cmake_minimum_required(VERSION 3.7)

project(SENTINET VERSION 0.0.1 LANGUAGES CXX)

# Some global path areas
set(CPP_DIR "${PROJECT_SOURCE_DIR}/cpp")
set(PYTHON_DIR "${PROJECT_SOURCE_DIR}/python")
set(TEMPLATES "${PROJECT_SOURCE_DIR}/resources/templates")

# using C++ 14
set(CMAKE_CXX_STANDARD 14)

# Versions
set(SENTINET_VERSION_MAJOR 0.0.1)

# Generate date time for when we tar it up
if(NOT DATE)
	string(TIMESTAMP DATE "%Y%m%d-%H%M%S")
endif()

# Some places to put our stuff
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/bin)

# Might remove this
include(GNUInstallDirs)

# Build documentation
option(BUILD_DOCS "Build the documentation" OFF)

# TODO Fix this
if(${BUILD_DOCS})
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        COMMENT "Generating API Documentation with Doxygen"
        VERBATIM)

    add_custom_command(TARGET doc_doxygen POST_BUILD
        COMMAND mkdir -p ${CMAKE_INSTALL_PREFIX}/docs)
    add_custom_command(TARGET doc_doxygen POST_BUILD
        COMMAND cp -r -u ${CMAKE_CURRENT_DIR}/docs ${CMAKE_INSTALL_PREFIX})

  else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed for doxygen documentation")
  endif(DOXYGEN_FOUND)
else()
  add_subdirectory(${CPP_DIR})
endif()


