# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(TrustWalletCoreProtobufPlugin)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version" FORCE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/../build/local")
else()
    set(PREFIX "$ENV{PREFIX}")
endif()

include_directories(${PREFIX}/include)
link_directories(${PREFIX}/lib)

find_package(Protobuf REQUIRED PATH ${PREFIX}/lib/pkgconfig)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(protoc-gen-c-typedef c_typedef.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-c-typedef protobuf -lprotoc -pthread)

add_executable(protoc-gen-swift-typealias swift_typealias.cc ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoc-gen-swift-typealias protobuf -lprotoc -pthread)

install(TARGETS protoc-gen-c-typedef protoc-gen-swift-typealias DESTINATION bin)
