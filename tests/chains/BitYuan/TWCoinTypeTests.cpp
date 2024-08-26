// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWBitYuanCoinType, TWCoinType) {
    const auto coin = TWCoinTypeBitYuan;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x08e67624a7a9fbec2af3ab37dac17b15a1facf56dd3806d7d84e95abb333a468"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x944880ad48b7e351513857701b39be968fd18d5b"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "bityuan");
    assertStringsEqual(name, "BitYuan Mainnet");
    assertStringsEqual(symbol, "BTY");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainBitYuan);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://mainnet.bityuan.com/tx/0x08e67624a7a9fbec2af3ab37dac17b15a1facf56dd3806d7d84e95abb333a468");
    assertStringsEqual(accUrl, "https://mainnet.bityuan.com/address/0x944880ad48b7e351513857701b39be968fd18d5b");
}
