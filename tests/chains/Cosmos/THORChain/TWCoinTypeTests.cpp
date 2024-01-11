// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWTHORChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTHORChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("ADF0899E58C177E2391F22D04E9C5E1C35BB0F75B42B363A0761687907FD9476"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTHORChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("thor196yf4pq80hjrmz7nnh0ar0ypqg02r0w4dq4mzu"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTHORChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTHORChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTHORChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTHORChain), 8);
    ASSERT_EQ(TWBlockchainThorchain, TWCoinTypeBlockchain(TWCoinTypeTHORChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTHORChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTHORChain));
    assertStringsEqual(symbol, "RUNE");
    assertStringsEqual(txUrl, "https://viewblock.io/thorchain/tx/ADF0899E58C177E2391F22D04E9C5E1C35BB0F75B42B363A0761687907FD9476");
    assertStringsEqual(accUrl, "https://viewblock.io/thorchain/address/thor196yf4pq80hjrmz7nnh0ar0ypqg02r0w4dq4mzu");
    assertStringsEqual(id, "thorchain");
    assertStringsEqual(name, "THORChain");
}
