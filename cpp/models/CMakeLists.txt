project(proto)

file(GLOB_RECURSE PROTO_FILES RELATIVE "${PROJECT_SOURCE_DIR}" "*.proto")
message(${PROTO_FILES})

set(proto_cmd "protoc")
set(input_dir "-I=.")
set(output_dir "--cpp_out=${INCLUDE_DIR}")

execute_process(
  COMMAND ${proto_cmd} ${PROTO_FILES} ${input_dir} ${output_dir}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
