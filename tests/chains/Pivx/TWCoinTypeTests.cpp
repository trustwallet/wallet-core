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


TEST(TWPivxCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePivx));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePivx));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePivx));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePivx), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypePivx));
    ASSERT_EQ(13, TWCoinTypeP2shPrefix(TWCoinTypePivx));
    ASSERT_EQ(30, TWCoinTypeP2pkhPrefix(TWCoinTypePivx));
    assertStringsEqual(symbol, "PIVX");
    assertStringsEqual(id, "pivx");
    assertStringsEqual(name, "Pivx");
}
