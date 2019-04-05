cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TrustWalletCoreProtobufPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/../build/local")
else()
    set(PREFIX "$ENV{PREFIX}")
endif()

include_directories(${PREFIX}/include)
link_directories(${PREFIX}/lib)

find_library(Protobuf REQUIRED PATH ${PREFIX}/lib/pkgconfig)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protoc-gen-c-typedef c_typedef.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-c-typedef protobuf -lprotoc -pthread)

add_executable(protoc-gen-swift-typealias swift_typealias.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-swift-typealias protobuf -lprotoc -pthread)
