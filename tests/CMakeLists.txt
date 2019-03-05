enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_SOURCE_DIR}/build/gtest/staging/googletest-release-1.8.1
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Protobuf
find_library(Protobuf REQUIRED PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)
include_directories(${Protobuf_INCLUDE_DIRS})

# Test executable
file(GLOB_RECURSE test_sources *.cpp **/*.cpp)
add_executable(tests ${test_sources})
target_compile_features(tests PRIVATE cxx_std_17)
target_link_libraries(tests gtest_main TrustWalletCore protobuf Boost::boost)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME example_test COMMAND tests)
