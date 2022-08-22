# Copyright © 2017-2022 Trust Wallet.
#
# This file is part of Trust. The full Trust copyright notice, including
# terms governing use, modification, and redistribution, is contained in the
# file LICENSE at the root of the source code distribution tree.

# walletconsole executable
file(GLOB walletconsole_sources *.cpp)
add_executable(walletconsole ${walletconsole_sources})
target_link_libraries(walletconsole walletconsolelib TrezorCrypto TrustWalletCore protobuf Boost::boost)
target_include_directories(walletconsole PRIVATE ${CMAKE_SOURCE_DIR}/walletconsole/lib ${CMAKE_SOURCE_DIR}/src)

INSTALL(TARGETS walletconsole DESTINATION ${CMAKE_INSTALL_BINDIR})
