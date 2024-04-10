// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWBlastCoinType, TWCoinType) {
    const auto coin = TWCoinTypeBlast;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x511fc00e8329343b9e953bf1f75e9b0a7b3cc2eb3a8f049d5be41adf4fbd6cac"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x0d11f2f0ff55c4fcfc3ff86bdc8e78ffa7df99fd"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "blast");
    assertStringsEqual(name, "Blast");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://blastscan.io/tx/0x511fc00e8329343b9e953bf1f75e9b0a7b3cc2eb3a8f049d5be41adf4fbd6cac");
    assertStringsEqual(accUrl, "https://blastscan.io/address/0x0d11f2f0ff55c4fcfc3ff86bdc8e78ffa7df99fd");
}
