project(proto)

file(GLOB_RECURSE PROTO_FILES RELATIVE "${PROJECT_SOURCE_DIR}" "*.proto")

set(proto_cmd "protoc")
set(input_dir "-I=.")
set(output_dir_cpp "--cpp_out=${CPP_SRC}/include")
set(output_dir_python "--python_out=${PYTHON_SRC}/src")

if(${BUILD_MODEL})
  execute_process(
    COMMAND ${proto_cmd} ${PROTO_FILES} ${input_dir} ${output_dir_cpp}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
  execute_process(
    COMMAND ${proto_cmd} ${PROTO_FILES} ${input_dir} ${output_dir_python}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()
