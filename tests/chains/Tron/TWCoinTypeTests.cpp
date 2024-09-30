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


TEST(TWTronCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTron, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);
    ASSERT_EQ(TWBlockchainTron, TWCoinTypeBlockchain(TWCoinTypeTron));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTron));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTron));
    assertStringsEqual(symbol, "TRX");
    assertStringsEqual(txUrl, "https://tronscan.org/#/transaction/t123");
    assertStringsEqual(accUrl, "https://tronscan.org/#/address/a12");
    assertStringsEqual(id, "tron");
    assertStringsEqual(name, "Tron");
}
