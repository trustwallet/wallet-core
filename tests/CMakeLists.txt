# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_SOURCE_DIR}/build/local/src/gtest/googletest-1.16.0
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Note: Protobuf is defined in included CMake
##find_library(Protobuf REQUIRED PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)
##include_directories(${Protobuf_INCLUDE_DIRS})

# Test executable
file(GLOB_RECURSE test_sources *.cpp **/*.cpp **/*.cc)
add_executable(tests ${test_sources})
target_link_libraries(tests gtest_main TrezorCrypto TrustWalletCore walletconsolelib protobuf Boost::boost)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/tests/common)
if (WIN32 AND MSVC)
    target_compile_options(tests PRIVATE /clang:-Wall)
else ()
    target_compile_options(tests PRIVATE -Wall)
endif ()
if (NOT ANDROID AND TW_UNITY_BUILD)
    set_target_properties(tests PROPERTIES UNITY_BUILD ON)
endif()

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(tests INTERFACE
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
    )
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(tests INTERFACE --coverage)
    else()
        target_link_libraries(tests INTERFACE --coverage)
    endif()
endif()

add_test(NAME example_test COMMAND tests)

if (WIN32)
    set_tests_properties(example_test PROPERTIES ENVIRONMENT "TW_TESTS_ROOT=${CMAKE_CURRENT_SOURCE_DIR}")
    add_executable(WindowsRandomTests windows/rand.c)
    target_include_directories(WindowsRandomTests PRIVATE ${CMAKE_SOURCE_DIR}/trezor-crypto/include)
    target_compile_definitions(WindowsRandomTests PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
endif ()
