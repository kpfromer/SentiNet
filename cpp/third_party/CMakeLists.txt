project(vendor)


if(${BUILD_THIRD_PARTY})
  add_subdirectory(yaml-cpp-0.6.0)
  add_subdirectory(cppzmq-4.4.1)
endif()
