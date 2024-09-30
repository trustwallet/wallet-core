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


TEST(TWIoTeXCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIoTeX));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIoTeX, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeIoTeX, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIoTeX));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIoTeX));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIoTeX), 18);
    ASSERT_EQ(TWBlockchainIoTeX, TWCoinTypeBlockchain(TWCoinTypeIoTeX));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeIoTeX));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeIoTeX));
    assertStringsEqual(symbol, "IOTX");
    assertStringsEqual(txUrl, "https://iotexscan.io/action/t123");
    assertStringsEqual(accUrl, "https://iotexscan.io/address/a12");
    assertStringsEqual(id, "iotex");
    assertStringsEqual(name, "IoTeX");
}
