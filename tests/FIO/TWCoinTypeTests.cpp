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


TEST(TWFIOCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFIO));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFIO, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFIO, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFIO));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFIO));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFIO), 9);
    ASSERT_EQ(TWBlockchainFIO, TWCoinTypeBlockchain(TWCoinTypeFIO));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeFIO));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFIO));
    assertStringsEqual(symbol, "FIO");
    assertStringsEqual(txUrl, "https://fio.foundation/?t123");
    assertStringsEqual(accUrl, "https://fio.foundation/?a12");
    assertStringsEqual(id, "fio");
    assertStringsEqual(name, "FIO");
}
