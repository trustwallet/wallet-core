enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(WIN32)
    find_package(GTest REQUIRED)
else()
    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_SOURCE_DIR}/build/local/src/gtest/googletest-release-1.10.0
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)
    set(GTEST_LIBRARIES gtest)
    set(GTEST_MAIN_LIBRARIES gtest_main)
endif()

# Note: Protobuf is defined in included CMake
##find_library(Protobuf REQUIRED PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)
##include_directories(${Protobuf_INCLUDE_DIRS})

# Test executable
file(GLOB_RECURSE test_sources *.cpp **/*.cpp)
add_executable(tests ${test_sources})
target_link_libraries(tests ${GTEST_MAIN_LIBRARIES} TrezorCrypto TrustWalletCore walletconsolelib ${Protobuf_LIBRARIES} Boost::boost)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
if(NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC"))
    target_compile_options(tests PRIVATE "-Wall")
endif()

set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 17
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
