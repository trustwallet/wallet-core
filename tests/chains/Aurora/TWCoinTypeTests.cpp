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


TEST(TWAuroraCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAurora));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x99deebdb70f8027037abb3d3d0f3c7523daee857d85e9056d2671593ff2f2f28"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAurora, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x8707cdE20dd43E3dB1F74c28fcd509ef38B0bA51"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAurora, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAurora));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAurora));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAurora), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeAurora));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAurora));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAurora));
    assertStringsEqual(symbol, "ETH");
    assertStringsEqual(txUrl, "https://aurorascan.dev/tx/0x99deebdb70f8027037abb3d3d0f3c7523daee857d85e9056d2671593ff2f2f28");
    assertStringsEqual(accUrl, "https://aurorascan.dev/address/0x8707cdE20dd43E3dB1F74c28fcd509ef38B0bA51");
    assertStringsEqual(id, "aurora");
    assertStringsEqual(name, "Aurora");
}
