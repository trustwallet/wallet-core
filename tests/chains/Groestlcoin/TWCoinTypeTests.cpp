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


TEST(TWGroestlcoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGroestlcoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeGroestlcoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGroestlcoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGroestlcoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGroestlcoin), 8);
    ASSERT_EQ(TWBlockchainGroestlcoin, TWCoinTypeBlockchain(TWCoinTypeGroestlcoin));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeGroestlcoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeGroestlcoin));
    assertStringsEqual(symbol, "GRS");
    assertStringsEqual(txUrl, "https://blockchair.com/groestlcoin/transaction/t123");
    assertStringsEqual(accUrl, "https://blockchair.com/groestlcoin/address/a12");
    assertStringsEqual(id, "groestlcoin");
    assertStringsEqual(name, "Groestlcoin");
}
