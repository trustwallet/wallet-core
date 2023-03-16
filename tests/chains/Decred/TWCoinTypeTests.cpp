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


TEST(TWDecredCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDecred));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDecred, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeDecred, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDecred));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDecred));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDecred), 8);
    ASSERT_EQ(TWBlockchainDecred, TWCoinTypeBlockchain(TWCoinTypeDecred));
    ASSERT_EQ(0x1a, TWCoinTypeP2shPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x7, TWCoinTypeStaticPrefix(TWCoinTypeDecred));
    assertStringsEqual(symbol, "DCR");
    assertStringsEqual(txUrl, "https://dcrdata.decred.org/tx/t123");
    assertStringsEqual(accUrl, "https://dcrdata.decred.org/address/a12");
    assertStringsEqual(id, "decred");
    assertStringsEqual(name, "Decred");
}
