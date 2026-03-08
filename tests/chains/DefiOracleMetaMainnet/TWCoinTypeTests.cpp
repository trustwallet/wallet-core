// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWDefiOracleMetaMainnetCoinType, TWCoinType) {
    const auto coin = TWCoinTypeDefiOracleMetaMainnet;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xbc36c6050ea6b6f484009a1fcd8f8c6c2c2bd629661bc75fa5e84829c662a604"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4A666F96fC8764181194447A7dFdb7d471b301C8"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "dfiometa");
    assertStringsEqual(name, "Defi Oracle Meta Mainnet");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.d-bis.org/tx/0xbc36c6050ea6b6f484009a1fcd8f8c6c2c2bd629661bc75fa5e84829c662a604");
    assertStringsEqual(accUrl, "https://explorer.d-bis.org/address/0x4A666F96fC8764181194447A7dFdb7d471b301C8");
}
