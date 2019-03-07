cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TrustWalletCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

include_directories(${CMAKE_SOURCE_DIR}/build/local/include)
link_directories(${CMAKE_SOURCE_DIR}/build/local/lib)

add_subdirectory(trezor-crypto)

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

# Source files
if(${ANDROID})
    message("Configuring for JNI")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h jni/cpp/*.c jni/cpp/*.cpp jni/cpp/*.h)
    add_library(TrustWalletCore SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    find_library(log-lib log)
    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf ${log-lib} Boost::boost)
else()
    message("Configuring standalone")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
    add_library(TrustWalletCore ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf Boost::boost)
endif()
set_property(TARGET TrustWalletCore PROPERTY CXX_STANDARD 17)
target_compile_options(TrustWalletCore PRIVATE "-Wall")

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

if(NOT ANDROID AND NOT IOS_PLATFORM)
    add_subdirectory(protobuf-plugin)
    add_subdirectory(tests)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig.in ${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig @ONLY)
