# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

enable_testing()

find_library(check PATH ${CMAKE_SOURCE_DIR}/build/local/lib/pkgconfig NO_DEFAULT_PATH)

# Test executable
add_executable(TrezorCryptoTests test_check.c)
target_link_libraries(TrezorCryptoTests TrezorCrypto check)
target_link_directories(TrezorCryptoTests PRIVATE ${PREFIX}/lib)
target_include_directories(TrezorCryptoTests PRIVATE ${CMAKE_SOURCE_DIR}/src ${PREFIX}/include)

add_test(NAME test_check COMMAND TrezorCryptoTests)
