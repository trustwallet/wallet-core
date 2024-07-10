// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWDchainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeDchain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xad07394f21b36d52fd4a55a2a36f3d7076d5cdb20d70dc73c14ef47cda597abf"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xCc04124d31f6B62829Dd4EDB51bFF6b7D5c2a76b"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "dchain");
    assertStringsEqual(name, "Dchain");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://dchain-2716446429837000-1.sagaexplorer.io/tx/0xad07394f21b36d52fd4a55a2a36f3d7076d5cdb20d70dc73c14ef47cda597abf");
    assertStringsEqual(accUrl, "https://dchain-2716446429837000-1.sagaexplorer.io/address/0xCc04124d31f6B62829Dd4EDB51bFF6b7D5c2a76b");
}
