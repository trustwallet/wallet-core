// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWXDCNetworkCoinType, TWCoinType) {
    const auto coin = TWCoinTypeXDCNetwork;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xbc4f1b0c59857dd97089d9d860f169d712c1fb6e"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "xdc");
    assertStringsEqual(name, "XDC Network");
    assertStringsEqual(symbol, "XDC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://xdc.network//tx/0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56");
    assertStringsEqual(accUrl, "https://xdc.network//address/0xbc4f1b0c59857dd97089d9d860f169d712c1fb6e");
}
