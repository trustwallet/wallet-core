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


TEST(TWVictionType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeViction));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x35a8d3ab06c94d5b7d27221b7c9a24ba3f1710dd0fcfd75c5d59b3a885fd709b"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeViction, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x86cCbD9bfb371c355202086882bC644A7D0b024B"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeViction, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeViction));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeViction));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeViction), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeViction));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeViction));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeViction));
    assertStringsEqual(symbol, "VIC");
    assertStringsEqual(txUrl, "https://www.vicscan.xyz/tx/0x35a8d3ab06c94d5b7d27221b7c9a24ba3f1710dd0fcfd75c5d59b3a885fd709b");
    assertStringsEqual(accUrl, "https://www.vicscan.xyz/address/0x86cCbD9bfb371c355202086882bC644A7D0b024B");
    assertStringsEqual(id, "viction");
    assertStringsEqual(name, "Viction");
}
