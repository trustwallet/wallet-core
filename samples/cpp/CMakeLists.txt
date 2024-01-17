# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

# Expected input configuration: WALLET_CORE: directory for TrustWalletCore build dir
# e.g. cmake . -DWALLET_CORE=../wallet-core
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)
project (wallet-core-demo-cpp)

if (NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    message(FATAL_ERROR "You should use clang compiler")
endif()

set (SETUP_MESSAGE "Please provide TrustWalletCore build directory with -DWALLET_CORE.   Example: cmake . -DWALLET_CORE=../wallet-core")

if (NOT WALLET_CORE)
    message (FATAL_ERROR "${SETUP_MESSAGE}")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)

# Include dirs:
# ${WALLET_CORE}/include -- public TrustWalletCore includes
# ${WALLET_CORE}/src -- internal TrustWalletCore files, for signer protobuf messages
# ${WALLET_CORE}/build/local/include) -- for protobuf includes
include_directories (${CMAKE_SOURCE_DIR} ${WALLET_CORE}/include ${WALLET_CORE}/src ${WALLET_CORE}/build/local/include)
link_directories (${WALLET_CORE}/build ${WALLET_CORE}/build/trezor-crypto ${WALLET_CORE}/build/local/lib ${WALLET_CORE}/rust/target/release)

find_library(WALLET_CORE_LIB_FILE TrustWalletCore PATH ${WALLET_CORE}/build)
if (NOT WALLET_CORE_LIB_FILE)
    message (FATAL_ERROR "TrustWalletCore library not found.  ${SETUP_MESSAGE}")
else ()
    message ("TrustWalletCore library found here: ${WALLET_CORE_LIB_FILE}")
endif ()

find_library(WALLET_CORE_RS_LIB_FILE wallet_core_rs PATH ${WALLET_CORE}/rust/target/release)
if (NOT WALLET_CORE_RS_LIB_FILE)
	message (FATAL_ERROR "wallet_core_rs library not found.  ${SETUP_MESSAGE}")
else ()
	message ("wallet_core_rs library found here: ${WALLET_CORE_RS_LIB_FILE}")
endif ()

# Create all libraries and executables in the root binary dir
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (NOT CMAKE_BUILD_TYPE)
	set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

if (WIN32)
	add_definitions (-D_WIN32_WINNT=0x0600
					-D_CRT_SECURE_NO_WARNINGS
					-DWINVER=0x0600
					-DWIN32_LEAN_AND_MEAN)
else ()
	add_compile_options (-Werror=switch)
endif ()

if (WIN32)
	add_definitions(/bigobj)
endif ()

if (APPLE)
	set (PLATFORM_LINK_FLAGS "-framework Foundation -framework OpenCL")
elseif (WIN32)
	set (PLATFORM_LINK_FLAGS "/STACK:8000000") #provides 8MB default stack size for linux on windows
endif ()

SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${PLATFORM_LINK_FLAGS}")

# sources of this exec
add_executable (sample sample.cpp)

# link with our library, and default platform libraries
target_link_libraries (sample PUBLIC TrustWalletCore ${WALLET_CORE_RS_LIB_FILE} TrezorCrypto protobuf pthread ${PLATFORM_LIBS})
