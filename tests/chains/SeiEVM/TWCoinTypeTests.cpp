// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWSeiEVMCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSeiEVM;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xe59dddcf9a93551399780d907eeee52fdbc44ead6406012368470d47979b7362"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xc5c8d37a73d96a925f770b5481f719df8d1a1b9e"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "seievm");
    assertStringsEqual(name, "Sei EVM");
    assertStringsEqual(symbol, "SEI");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://seiscan.io/tx/0xe59dddcf9a93551399780d907eeee52fdbc44ead6406012368470d47979b7362");
    assertStringsEqual(accUrl, "https://seiscan.io/address/0xc5c8d37a73d96a925f770b5481f719df8d1a1b9e");
}
