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


TEST(TWSuiCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSui;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("5i8fbSL6r8yw2xcKmXxwkzHu3wpiyMLsyf2htCvDH8Ao"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "sui");
    assertStringsEqual(name, "Sui");
    assertStringsEqual(symbol, "SUI");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 9);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainSui);
    assertStringsEqual(txUrl, "https://explorer.sui.io//txblock/5i8fbSL6r8yw2xcKmXxwkzHu3wpiyMLsyf2htCvDH8Ao");
    assertStringsEqual(accUrl, "https://explorer.sui.io//address/0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015");
}
