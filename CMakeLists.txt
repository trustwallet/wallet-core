# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(TrustWalletCore)

if (NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    message(FATAL_ERROR "You should use clang compiler")
endif ()

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/build/local")
else ()
    set(PREFIX "$ENV{PREFIX}")
endif ()

include(GNUInstallDirs)
include(cmake/StandardSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/FindHostPackage.cmake)

add_library(${PROJECT_NAME}_INTERFACE INTERFACE)
target_include_directories(${PROJECT_NAME}_INTERFACE INTERFACE ${PREFIX}/include)
target_link_directories(${PROJECT_NAME}_INTERFACE INTERFACE ${PREFIX}/lib)
set_project_warnings(${PROJECT_NAME}_INTERFACE)

add_subdirectory(trezor-crypto)
if (TW_COMPILE_WASM)
    message(STATUS "Wasm build enabled")
    add_subdirectory(wasm)
endif ()

find_host_package(Boost REQUIRED)

include(ExternalProject)

# Dependencies
include(cmake/Protobuf.cmake)

# Source files
if (${ANDROID})
    message("Configuring for JNI")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h jni/cpp/*.c jni/cpp/*.cpp jni/cpp/*.h jni/cpp/*.c)
    add_library(TrustWalletCore SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})
    find_library(log-lib log)
    target_link_libraries(TrustWalletCore PUBLIC ${PROJECT_NAME}_INTERFACE PRIVATE TrezorCrypto protobuf ${log-lib} Boost::boost)
else ()
    message("Configuring standalone")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
    add_library(TrustWalletCore ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    target_link_libraries(TrustWalletCore PUBLIC ${PROJECT_NAME}_INTERFACE PRIVATE TrezorCrypto protobuf Boost::boost)
endif ()

if (TW_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_enable_coverage(TrustWalletCore)
endif ()

if (TW_CLANG_ASAN)
    target_enable_asan(TrustWalletCore)
endif ()

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

if (TW_UNIT_TESTS)
    add_subdirectory(tests)
endif ()

if (TW_BUILD_EXAMPLES)
    add_subdirectory(walletconsole/lib)
    add_subdirectory(walletconsole)
endif ()

if (TW_ENABLE_PVS_STUDIO)
    tw_add_pvs_studio_target(TrustWalletCore)
endif ()

if (TW_ENABLE_CLANG_TIDY)
    tw_add_clang_tidy_target(TrustWalletCore)
endif ()

if (NOT ANDROID AND TW_UNITY_BUILD)
    set_target_properties(TrustWalletCore PROPERTIES UNITY_BUILD ON)

    file(GLOB_RECURSE PROTOBUF_SOURCE_FILES CONFIGURE_DEPENDS src/Cosmos/Protobuf/*.pb.cc src/Hedera/Protobuf/*.pb.cc src/proto/*.pb.cc)
    foreach(file ${PROTOBUF_SOURCE_FILES})
        set_property(SOURCE ${file} PROPERTY SKIP_UNITY_BUILD_INCLUSION ON)
    endforeach()
    message(STATUS "Unity build activated")
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
