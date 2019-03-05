enable_testing()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/googletest
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Protobuf
find_library(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# Test executable
file(GLOB_RECURSE test_sources *.cpp **/*.cpp)
add_executable(tests ${test_sources})
target_compile_features(tests PRIVATE cxx_std_14)
target_link_libraries(tests gtest_main TrustWalletCore protobuf Boost::boost)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME example_test COMMAND tests)
