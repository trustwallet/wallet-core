cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TrustWalletCoreProtobufPlugin)

set (CMAKE_CXX_STANDARD 14)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protoc-gen-int main.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-int ${Protobuf_LIBRARIES} ${Protobuf_PROTOC_LIBRARIES})
