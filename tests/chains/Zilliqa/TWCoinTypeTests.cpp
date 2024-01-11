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


TEST(TWZilliqaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZilliqa));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZilliqa, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZilliqa, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZilliqa));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZilliqa));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZilliqa), 12);
    ASSERT_EQ(TWBlockchainZilliqa, TWCoinTypeBlockchain(TWCoinTypeZilliqa));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeZilliqa));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeZilliqa));
    assertStringsEqual(symbol, "ZIL");
    assertStringsEqual(txUrl, "https://viewblock.io/zilliqa/tx/t123");
    assertStringsEqual(accUrl, "https://viewblock.io/zilliqa/address/a12");
    assertStringsEqual(id, "zilliqa");
    assertStringsEqual(name, "Zilliqa");
}
