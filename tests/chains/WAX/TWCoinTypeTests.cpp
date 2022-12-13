// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWWAXCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWAX));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("testtx"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWAX, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("testacc"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeWAX, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWAX));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWAX));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWAX), 8);
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeWAX));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeWAX));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeWAX));
    assertStringsEqual(symbol, "WAXP");
    assertStringsEqual(txUrl, "https://bloks.io/transaction/testtx");
    assertStringsEqual(accUrl, "https://bloks.io/account/testacc");
    assertStringsEqual(id, "wax");
    assertStringsEqual(name, "WAX");
}
