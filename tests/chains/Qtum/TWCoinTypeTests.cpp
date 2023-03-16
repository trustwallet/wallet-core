// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWQtumCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeQtum, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeQtum));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeQtum));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeQtum), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeQtum));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeQtum));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeQtum));
    assertStringsEqual(symbol, "QTUM");
    assertStringsEqual(txUrl, "https://qtum.info/tx/t123");
    assertStringsEqual(accUrl, "https://qtum.info/address/a12");
    assertStringsEqual(id, "qtum");
    assertStringsEqual(name, "Qtum");
}
