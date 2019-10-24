// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWNEOCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEO));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEO, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEO));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEO));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEO), 8);
    ASSERT_EQ(TWBlockchainNEO, TWCoinTypeBlockchain(TWCoinTypeNEO));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNEO));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNEO));
    assertStringsEqual(symbol, "NEO");
    assertStringsEqual(txUrl, "https://neoscan.io/transaction/123");
    assertStringsEqual(id, "neo");
    assertStringsEqual(name, "NEO");
}
