cmake_minimum_required (VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -std=c++11")  # disable warning messages for temporary debugging usage
################################################################
# gRPC
################################################################
find_package(Protobuf REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(PROTO_PATH "${CMAKE_SOURCE_DIR}/protos")
set(LOCATION_PROTO "${PROTO_PATH}/location.proto")
set(GENERATED_PROTOBUF_PATH "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTOBUF_PATH})
message("CMAKE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:${PROTO_PATH}")

set(LOCATION_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/location.pb.cc")
set(LOCATION_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/location.pb.h")
set(LOCATION_GRPC_PB_CPP_FILE "${GENERATED_PROTOBUF_PATH}/location.grpc.pb.cc")
set(LOCATION_GRPC_PB_H_FILE "${GENERATED_PROTOBUF_PATH}/location.grpc.pb.h")
add_custom_command(
    OUTPUT "${LOCATION_PB_H_FILE}"
           "${LOCATION_PB_CPP_FILE}"
           "${LOCATION_GRPC_PB_H_FILE}"
           "${LOCATION_GRPC_PB_CPP_FILE}"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS "--proto_path=${PROTO_PATH}"
         "--cpp_out=${GENERATED_PROTOBUF_PATH}"
         "${LOCATION_PROTO}"
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS "--proto_path=${PROTO_PATH}"
         "--grpc_out=${GENERATED_PROTOBUF_PATH}"
         "--plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin"
         "${LOCATION_PROTO}"
    )
set(GENERATED_PROTOBUF_FILES ${LOCATION_PB_H_FILE} ${LOCATION_PB_CPP_FILE}
    ${LOCATION_GRPC_PB_H_FILE} ${LOCATION_GRPC_PB_CPP_FILE})

include_directories(${GENERATED_PROTOBUF_PATH})
link_libraries(grpc++_unsecure grpc gpr ${PROTOBUF_LIBRARY})


################################################################
add_executable(location location.cpp ${GENERATED_PROTOBUF_FILES} s6a_task.cpp s6a_up_loc.cpp s6a_config.cpp s6a_dict.cpp s6a_peer.cpp s6a_error.cpp s6a_subscription_data.cpp bstrlib.cpp backtrace.cpp)
target_link_libraries (location fdproto fdcore gnutls config)
