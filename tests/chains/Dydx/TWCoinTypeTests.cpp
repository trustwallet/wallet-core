// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWDydxCoinType, TWCoinType) {
    const auto coin = TWCoinTypeDydx;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("F236222E4F7C92FA84711FD6451ED22DD56CBDFA319BFDAFB99A21E4E9B9EC2F"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "dydx");
    assertStringsEqual(name, "dYdX");
    assertStringsEqual(symbol, "DYDX");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://www.mintscan.io/dydx/tx/F236222E4F7C92FA84711FD6451ED22DD56CBDFA319BFDAFB99A21E4E9B9EC2F");
    assertStringsEqual(accUrl, "https://www.mintscan.io/dydx/address/dydx1adl7usw7z2dnysyn7wvrghu0u0q6gr7jqs4gtt");
}
