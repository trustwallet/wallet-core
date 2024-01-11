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


TEST(TWMantleCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMantle;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xfae996ea23f1ff9909ac04d26ae6e52ab600a84163fab9e0e893483c685629dd"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xA295EEFd401C8BE1457F266d3e73cdD015e5CFbb"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "mantle");
    assertStringsEqual(name, "Mantle");
    assertStringsEqual(symbol, "MNT");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "5000");
    assertStringsEqual(txUrl, "https://explorer.mantle.xyz/tx/0xfae996ea23f1ff9909ac04d26ae6e52ab600a84163fab9e0e893483c685629dd");
    assertStringsEqual(accUrl, "https://explorer.mantle.xyz/address/0xA295EEFd401C8BE1457F266d3e73cdD015e5CFbb");
}
