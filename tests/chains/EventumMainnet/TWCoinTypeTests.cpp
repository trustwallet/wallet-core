// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWEventumMainnetCoinType, TWCoinType) {
    const auto coin = TWCoinTypeEventumMainnet;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xfb74c2f7645f0add57f4d1bd3a4184c041dbcb7424fbdadd0d0f9e1f4eee2406"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x658B13b773b0ceD400eC57cf7C03288d8Aa13805"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "eventum");
    assertStringsEqual(name, "Eventum Mainnet");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.evedex.com//tx/0xfb74c2f7645f0add57f4d1bd3a4184c041dbcb7424fbdadd0d0f9e1f4eee2406");
    assertStringsEqual(accUrl, "https://explorer.evedex.com//address/0x658B13b773b0ceD400eC57cf7C03288d8Aa13805");
}
