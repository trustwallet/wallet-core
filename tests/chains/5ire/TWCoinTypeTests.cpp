// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TW5ireCoinType, TWCoinType) {
    const auto coin = TW5ireCoinType;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x93ea92687845fe7bb6cacd69c76a16a2a3c2bbb85a8a93ff0e032d0098d583d7"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x2ad9656bf5b82caf10847b431012e28e301e83ba"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "5ire");
    assertStringsEqual(name, "5ire");
    assertStringsEqual(symbol, "5ire");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "995");
    assertStringsEqual(txUrl, "https://5irescan.io/tx/0x93ea92687845fe7bb6cacd69c76a16a2a3c2bbb85a8a93ff0e032d0098d583d7");
    assertStringsEqual(accUrl, "https://5irescan.io/account/0x2ad9656bf5b82caf10847b431012e28e301e83ba");
