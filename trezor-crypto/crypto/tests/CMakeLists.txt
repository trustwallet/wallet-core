# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

enable_testing()

find_library(check PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)

# Test executable
add_executable(TrezorCryptoTests test_check.c)
target_link_libraries(TrezorCryptoTests TrezorCrypto check)
target_link_directories(TrezorCryptoTests PRIVATE ${PREFIX}/lib)
target_include_directories(TrezorCryptoTests PRIVATE ${CMAKE_SOURCE_DIR}/src ${PREFIX}/include)

add_test(NAME test_check COMMAND TrezorCryptoTests)
