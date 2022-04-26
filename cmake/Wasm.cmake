# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TrustWalletCore)

include(GNUInstallDirs)

message(STATUS "Building for emscripten")

# Configure warnings
set(TW_CXX_WARNINGS "-Wshorten-64-to-32")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TW_CXX_WARNINGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/build/local")
else()
    set(PREFIX "$ENV{PREFIX}")
endif()

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

include_directories(${PREFIX}/include)
link_directories(${PREFIX}/lib)

add_subdirectory(trezor-crypto)
add_subdirectory(wasm)

find_package(Boost)

include(cmake/Protobuf.cmake)

file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
add_library(TrustWalletCore ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf Boost::boost)
target_compile_options(TrustWalletCore PRIVATE "-Wall")

set_target_properties(TrustWalletCore
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(TrustWalletCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/build/local/include
)

install(TARGETS TrustWalletCore
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/WalletCore
    FILES_MATCHING PATTERN "*.h"
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
