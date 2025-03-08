// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWBricsChainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeBricsChain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x93ea92687845fe7bb6cacd69c76a16a2a3c2bbb85a8a93ff0e032d0098d583d7"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x2ad9656bf5b82caf10847b431012e28e301e83ba"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "bricschain");
    assertStringsEqual(name, "BricsChain");
    assertStringsEqual(symbol, "BRICS");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://scanbrics.com/tx/0x93ea92687845fe7bb6cacd69c76a16a2a3c2bbb85a8a93ff0e032d0098d583d7");
    assertStringsEqual(accUrl, "https://scanbrics.com/address/0x2ad9656bf5b82caf10847b431012e28e301e83ba");
}
