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


TEST(TWViacoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeViacoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeViacoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeViacoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeViacoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeViacoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeViacoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeViacoin));
    ASSERT_EQ(0x21, TWCoinTypeP2shPrefix(TWCoinTypeViacoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeViacoin));
    assertStringsEqual(symbol, "VIA");
    assertStringsEqual(txUrl, "https://explorer.viacoin.org/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.viacoin.org/address/a12");
    assertStringsEqual(id, "viacoin");
    assertStringsEqual(name, "Viacoin");
}
