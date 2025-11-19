// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWStacksCoinType, TWCoinType) {
    const auto coin = TWCoinTypeStacks;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x2aeb646c13a0261ebc02003877d2db6c839d3bcbeea6ba7ede877f484f6ce70c"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("SP3XXK8BG5X7CRH7W07RRJK3JZJXJ799WX3Y0SMCR"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "stacks");
    assertStringsEqual(name, "Stacks");
    assertStringsEqual(symbol, "STX");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 8);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainStacks);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 1);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.hiro.so/txid/0x2aeb646c13a0261ebc02003877d2db6c839d3bcbeea6ba7ede877f484f6ce70c");
    assertStringsEqual(accUrl, "https://explorer.hiro.so/address/SP3XXK8BG5X7CRH7W07RRJK3JZJXJ799WX3Y0SMCR");
}
