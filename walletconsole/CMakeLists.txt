# SPDX-License-Identifier: Apache-2.0
#
# Copyright © 2017 Trust Wallet.

# walletconsole executable
file(GLOB walletconsole_sources *.cpp)
add_executable(walletconsole ${walletconsole_sources})
target_link_libraries(walletconsole walletconsolelib TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_include_directories(walletconsole PRIVATE ${CMAKE_SOURCE_DIR}/walletconsole/lib ${CMAKE_SOURCE_DIR}/src)

INSTALL(TARGETS walletconsole DESTINATION ${CMAKE_INSTALL_BINDIR})
