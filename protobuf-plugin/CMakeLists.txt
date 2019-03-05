cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TrustWalletCoreProtobufPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

include_directories(${CMAKE_SOURCE_DIR}/build/local/include)
link_directories(${CMAKE_SOURCE_DIR}/build/local/lib)

find_library(Protobuf REQUIRED PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protoc-gen-int main.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-int protobuf protoc)
