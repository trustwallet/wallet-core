# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

# walletconsolelib library
file(GLOB_RECURSE walletconsolelib_sources *.cpp)
add_library(walletconsolelib ${walletconsolelib_sources})
#target_link_libraries(tests gtest_main TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_link_libraries(walletconsolelib TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_include_directories(walletconsolelib PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
