# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

# walletconsolelib library
file(GLOB_RECURSE walletconsolelib_sources *.cpp)
add_library(walletconsolelib ${walletconsolelib_sources})
#target_link_libraries(tests gtest_main TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_link_libraries(walletconsolelib TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_include_directories(walletconsolelib PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
