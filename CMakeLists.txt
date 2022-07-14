# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TrustWalletCore)

include(GNUInstallDirs)
include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)

# Configure warnings
set(TW_CXX_WARNINGS "-Wshorten-64-to-32")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TW_CXX_WARNINGS}")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version" FORCE)

if (NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    message(FATAL_ERROR "You should use clang compiler")
endif()

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
if (TW_COMPILE_WASM)
    message(STATUS "Wasm build enabled")
    add_subdirectory(wasm)
endif()

macro(find_host_package)
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
  find_package(${ARGN})
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro(find_host_package)

find_host_package(Boost REQUIRED)

include(ExternalProject)

# Dependencies
include(cmake/Protobuf.cmake)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

option(CLANG_ASAN "Enable ASAN dynamic address sanitizer" OFF)
if(CLANG_ASAN)
    # https://clang.llvm.org/docs/AddressSanitizer.html
    # https://github.com/trustwallet/wallet-core/issues/1170
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
    message("CLANG_ASAN on, ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

# Source files
if(${ANDROID})
    message("Configuring for JNI")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h jni/cpp/*.c jni/cpp/*.cpp jni/cpp/*.h jni/cpp/*.c)
    add_library(TrustWalletCore SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    find_library(log-lib log)
    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf ${log-lib} Boost::boost)
else()
    message("Configuring standalone")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
    add_library(TrustWalletCore ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf Boost::boost)
endif()
target_compile_options(TrustWalletCore PRIVATE "-Wall")

set_target_properties(TrustWalletCore
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(TrustWalletCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/build/local/include
)

if(NOT ANDROID AND NOT IOS_PLATFORM AND NOT TW_COMPILE_WASM)
    add_subdirectory(tests)
    add_subdirectory(walletconsole/lib)
    add_subdirectory(walletconsole)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig.in ${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig @ONLY)

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
