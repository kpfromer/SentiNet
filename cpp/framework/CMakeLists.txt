project(framework)


file(GLOB COMMON_CLIENT_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/client/*.cpp")
file(GLOB COMMON_SERVER_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/server/*.cpp")
file(GLOB COMMON_CURL_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/curl/*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/include)

if(BUILD_SHARED)
  add_library(zmqclient SHARED ${COMMON_CLIENT_SOURCE_FILES})
  add_library(zmqserver SHARED ${COMMON_SERVER_SOURCE_FILES})
  add_library(curlloader SHARED ${COMMON_CURL_SOURCE_FILES})
else(BUILD_SHARED)
  add_library(zmqclient STATIC ${COMMON_CLIENT_SOURCE_FILES})
  add_library(zmqserver STATIC ${COMMON_SERVER_SOURCE_FILES})
  add_library(curlloader STATIC ${COMMON_CURL_SOURCE_FILES})
endif()


find_library(LIB_ZMQ zmq)
find_library(LIB_PTHREAD pthread)
find_library(LIB_CURL curl)

target_link_libraries(zmqclient ${LIB_ZMQ})
target_link_libraries(zmqserver ${LIB_ZMQ} ${LIB_PTHREAD})
target_link_libraries(curlloader ${LIB_CURL})


#set_target_properties(zmqclient PROPERTIES
#VERSION ${IIRF_VERSION_MAJOR}
#SOVERSION 1
#PUBLIC_HEADER include/networking/client/ZMQClient.hpp)
#set_target_properties(zmqserver PROPERTIES
#VERSION ${IIRF_VERSION_MAJOR}
#SOVERSION 1
#PUBLIC_HEADER include/networking/server/ZMQServer.hpp)
#
#
#install(TARGETS zmqclient zmqserver
#EXPORT ${PROJECT_NAME}-targets
#LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
#
#install(DIRECTORY include
#DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#FILES_MATCHING PATTERN "*.hpp")
