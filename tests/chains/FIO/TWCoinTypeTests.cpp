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


TEST(TWFIOCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFIO));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("930d1d3cf8988b39b5f64b64e9d61314a3e05a155d9e3505bdf863aab1adddf3"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFIO, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("f5axfpgffiqz"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFIO, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFIO));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFIO));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFIO), 9);
    ASSERT_EQ(TWBlockchainFIO, TWCoinTypeBlockchain(TWCoinTypeFIO));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeFIO));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFIO));
    assertStringsEqual(symbol, "FIO");
    assertStringsEqual(txUrl, "https://explorer.fioprotocol.io/transaction/930d1d3cf8988b39b5f64b64e9d61314a3e05a155d9e3505bdf863aab1adddf3");
    assertStringsEqual(accUrl, "https://explorer.fioprotocol.io/account/f5axfpgffiqz");
    assertStringsEqual(id, "fio");
    assertStringsEqual(name, "FIO");
}
