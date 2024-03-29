cmake_minimum_required(VERSION 3.7)

# Project attribtues
project(SENTINET VERSION 0.0.1 LANGUAGES C CXX)

# Some global paths 
set(CPP_SRC "${PROJECT_SOURCE_DIR}/cpp")
set(PYTHON_SRC "${PROJECT_SOURCE_DIR}/python")
set(MODELS_SRC "${PROJECT_SOURCE_DIR}/models")

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
include(${PROJECT_SOURCE_DIR}/.cmake/cpp.cmake)
include(${PROJECT_SOURCE_DIR}/.cmake/deps.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Some output directories
set(BIN_DIR "bin")
set(SRC_DIR "src")
set(INC_DIR "include")
set(LIB_DIR "lib")
set(ARCHIVE_DIR "lib")
set(DOC_DIR "docs")

set(CMAKE_INSTALL_PREFIX /opt/sentinet)

set(CMAKE_INSTALL_ROOTDIR ${CMAKE_INSTALL_PREFIX})
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_ROOTDIR}/${BIN_DIR})
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_ROOTDIR}/${LIB_DIR})
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_ROOTDIR}/${DOC_DIR})

# Some places to put our stuff
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/x86_64/bin)


# Build documentation
option(BUILD_DOCS "Build the documentation" OFF)
option(BUILD_DEPENDENCIES "Build external dependencies" OFF)
option(BUILD_MODEL "Generate model files from protobuf" OFF)

# TODO Fix this
if(${BUILD_DOCS})
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    message("Doxygen build started")

    add_custom_target( doc_doxygen ALL
        COMMAND mkdir -p ${DOXYGEN_OUT}
        COMMENT "Generating API Documentation with Doxygen"
        VERBATIM)

    add_custom_command(TARGET doc_doxygen POST_BUILD
      COMMAND mkdir -p ${CMAKE_INSTALL_DOCDIR}/docs)
    add_custom_command(TARGET doc_doxygen POST_BUILD
      COMMAND cp -r -u ${CMAKE_CURRENT_DIR}/docs ${CMAKE_INSTALL_DOCDIR})

  else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed for doxygen documentation")
  endif(DOXYGEN_FOUND)
else()
  add_subdirectory(${MODELS_SRC})
  add_subdirectory(${CPP_SRC})
endif()


