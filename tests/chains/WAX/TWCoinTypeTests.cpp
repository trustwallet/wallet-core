// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWWAXCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWAX));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("4548f7b28ee608663caea61234049ac0018415e02dd0abcea1c215c8da00d10a"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWAX, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("k52o1qdeh.gm"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeWAX, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWAX));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWAX));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWAX), 4);
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeWAX));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeWAX));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeWAX));
    assertStringsEqual(symbol, "WAXP");
    assertStringsEqual(txUrl, "https://wax.bloks.io/transaction/4548f7b28ee608663caea61234049ac0018415e02dd0abcea1c215c8da00d10a");
    assertStringsEqual(accUrl, "https://wax.bloks.io/account/k52o1qdeh.gm");
    assertStringsEqual(id, "wax");
    assertStringsEqual(name, "WAX");
}
