cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TrustWalletCoreProtobufPlugin)

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/../build/local")
else()
    set(PREFIX "$ENV{PREFIX}")
endif()

include_directories(${PREFIX}/include)
link_directories(${PREFIX}/lib)

find_library(Protobuf REQUIRED PATH ${PREFIX}/lib/pkgconfig NO_DEFAULT_PATH)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protoc-gen-int main.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-int protobuf protoc)
