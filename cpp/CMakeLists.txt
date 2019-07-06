project(cpp)

set(COMMON_DIR "${PROJECT_SOURCE_DIR}/common")
set(IMPL_DIR "${PROJECT_SOURCE_DIR}/impl")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
set(CONFIG_DIR "${PROJECT_SOURCE_DIR}/config")


#Adding config file to highest directory rather than just add_subdirectory because
#config is really small
configure_file(
  "${CONFIG_DIR}/IIRFConfig.h.in"
  "${CONFIG_DIR}/IIRFConfig.h"
  )

include_directories(${IIRF_CONFIG_DIR})

#add_subdirectory(tests)
add_subdirectory(${COMMON_DIR})
add_subdirectory(${IMPL_DIR})
