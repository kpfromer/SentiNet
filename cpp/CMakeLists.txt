project(cpp)

set(FRAMEWORK_DIR "${PROJECT_SOURCE_DIR}/framework")
#set(IMPL_DIR "${PROJECT_SOURCE_DIR}/impl")
set(PROTO_DIR "${PROJECT_SOURCE_DIR}/protoc")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
#set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(CONFIG_DIR "${PROJECT_SOURCE_DIR}/config")


#Adding config file to highest directory rather than just add_subdirectory because
#config is really small
configure_file(
  "${CONFIG_DIR}/IIRFConfig.h.in"
  "${CONFIG_DIR}/IIRFConfig.h"
  )

include_directories(${IIRF_CONFIG_DIR})

#ADD_SUBDIRECTORY(TESTS)
add_subdirectory(${FRAMEWORK_DIR})
add_subdirectory(${PROTO_DIR})
add_subdirectory(${TESTS_DIR})

