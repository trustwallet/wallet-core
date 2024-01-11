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


TEST(TWNebulasCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNebulas));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNebulas, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNebulas, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNebulas));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNebulas));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNebulas), 18);
    ASSERT_EQ(TWBlockchainNebulas, TWCoinTypeBlockchain(TWCoinTypeNebulas));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNebulas));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNebulas));
    assertStringsEqual(symbol, "NAS");
    assertStringsEqual(txUrl, "https://explorer.nebulas.io/#/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.nebulas.io/#/address/a12");
    assertStringsEqual(id, "nebulas");
    assertStringsEqual(name, "Nebulas");
}
