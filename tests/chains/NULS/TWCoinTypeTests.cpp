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


TEST(TWNULSCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNULS));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("303e0e42c28acc37ba952a1effd43daa1caec79928054f7abefb21c32e6fdc02"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNULS, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("NULSd6HgdSjUZy7jKMZfvQ5QU6Z97oufGTGcF"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNULS, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNULS));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNULS));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNULS), 8);
    ASSERT_EQ(TWBlockchainNULS, TWCoinTypeBlockchain(TWCoinTypeNULS));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNULS));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNULS));
    assertStringsEqual(symbol, "NULS");
    assertStringsEqual(txUrl, "https://nulscan.io/transaction/info?hash=303e0e42c28acc37ba952a1effd43daa1caec79928054f7abefb21c32e6fdc02");
    assertStringsEqual(accUrl, "https://nulscan.io/address/info?address=NULSd6HgdSjUZy7jKMZfvQ5QU6Z97oufGTGcF");
    assertStringsEqual(id, "nuls");
    assertStringsEqual(name, "NULS");
}
