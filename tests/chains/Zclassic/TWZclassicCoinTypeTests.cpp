// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWZclassicCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZclassic));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("f2438a93039faf08d39bd3df1f7b5f19a2c29ffe8753127e2956ab4461adab35"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZclassic, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("t1Yfrf1dssDLmaMBsq2LFKWPbS5vH3nGpa2"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZclassic, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZclassic));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZclassic));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZclassic), 8);
    ASSERT_EQ(TWBlockchainZclassic, TWCoinTypeBlockchain(TWCoinTypeZclassic));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZclassic));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZclassic));
    assertStringsEqual(symbol, "ZCL");
    assertStringsEqual(txUrl, "https://explorer.zcl.zelcore.io/tx/f2438a93039faf08d39bd3df1f7b5f19a2c29ffe8753127e2956ab4461adab35");
    assertStringsEqual(accUrl, "https://explorer.zcl.zelcore.io/address/t1Yfrf1dssDLmaMBsq2LFKWPbS5vH3nGpa2");
    assertStringsEqual(id, "zclassic");
    assertStringsEqual(name, "Zclassic");
}
