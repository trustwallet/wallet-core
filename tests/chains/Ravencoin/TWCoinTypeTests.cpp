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


TEST(TWRavencoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRavencoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRavencoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeRavencoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRavencoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRavencoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRavencoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeRavencoin));
    ASSERT_EQ(0x7a, TWCoinTypeP2shPrefix(TWCoinTypeRavencoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeRavencoin));
    assertStringsEqual(symbol, "RVN");
    assertStringsEqual(txUrl, "https://blockbook.ravencoin.org/tx/t123");
    assertStringsEqual(accUrl, "https://blockbook.ravencoin.org/address/a12");
    assertStringsEqual(id, "ravencoin");
    assertStringsEqual(name, "Ravencoin");
}
