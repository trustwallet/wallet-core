// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWSonicCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSonic;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x886c34de9e054c741b2bcb15c3a3e0382e3ed7a48f2c6f2a489f6d82abdd4a7c"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9c174f0e2d11559447e5fe2815d930475be19016"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "sonic");
    assertStringsEqual(name, "Sonic");
    assertStringsEqual(symbol, "S");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://sonicscan.org/tx/0x886c34de9e054c741b2bcb15c3a3e0382e3ed7a48f2c6f2a489f6d82abdd4a7c");
    assertStringsEqual(accUrl, "https://sonicscan.org/address/0x9c174f0e2d11559447e5fe2815d930475be19016");
}
