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


TEST(TWNimiqCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNimiq));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNimiq, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNimiq, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNimiq));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNimiq));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNimiq), 5);
    ASSERT_EQ(TWBlockchainNimiq, TWCoinTypeBlockchain(TWCoinTypeNimiq));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNimiq));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNimiq));
    assertStringsEqual(symbol, "NIM");
    assertStringsEqual(txUrl, "https://nimiq.watch/#t123");
    assertStringsEqual(accUrl, "https://nimiq.watch/#a12");
    assertStringsEqual(id, "nimiq");
    assertStringsEqual(name, "Nimiq");
}
