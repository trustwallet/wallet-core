// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWPolymeshCoinType, TWCoinType) {
    const auto coin = TWCoinTypePolymesh;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("2E5u4xA1TqswQ3jMJH96zekxwr2itvKu79fDC1mmnVZRh6Uv"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "polymesh");
    assertStringsEqual(name, "Polymesh");
    assertStringsEqual(symbol, "POLYX");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainPolymesh);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://polymesh.subscan.io/extrinsic/0x98cb5e33d8ff3dd5838c384e2ef9e291314ed8db13f5d4f42cdd70bad54a5e04");
    assertStringsEqual(accUrl, "https://polymesh.subscan.io/account/2E5u4xA1TqswQ3jMJH96zekxwr2itvKu79fDC1mmnVZRh6Uv");
}
