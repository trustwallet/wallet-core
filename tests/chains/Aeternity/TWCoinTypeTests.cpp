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


TEST(TWAeternityCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAeternity));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAeternity, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAeternity, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAeternity));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAeternity));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAeternity), 18);
    ASSERT_EQ(TWBlockchainAeternity, TWCoinTypeBlockchain(TWCoinTypeAeternity));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAeternity));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAeternity));
    assertStringsEqual(symbol, "AE");
    assertStringsEqual(txUrl, "https://explorer.aepps.com/transactions/t123");
    assertStringsEqual(accUrl, "https://explorer.aepps.com/account/transactions/a12");
    assertStringsEqual(id, "aeternity");
    assertStringsEqual(name, "Aeternity");
}
