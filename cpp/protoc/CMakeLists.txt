
cmake_minimum_required(VERSION 3.5)
project(proto)

include_directories(include)

execute_process(
    COMMAND bash -c "protoc -I=${PROJECT_SOURCE_DIR}/proto --cpp_out=${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/proto/*.proto"
    )
execute_process(
    COMMAND bash -c "mv ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/include"
    )


set(DRIVE_TRAIN_SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/drivetrain.pb.cc")

if(BUILD_SHARED)
    add_library(dtproto SHARED ${DRIVE_TRAIN_SOURCE_FILES})
else(BUILD_SHARED)
    add_library(dtproto STATIC ${DRIVE_TRAIN_SOURCE_FILES})
endif()


