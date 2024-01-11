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


TEST(TWMetisCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMetis));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x422f2ebbede32d4434ad0cf0ae55d44a84e14d3d5725a760133255b42676d8ce"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMetis, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xBe9E8Ec25866B21bA34e97b9393BCabBcB4A5C86"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMetis, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMetis));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMetis));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMetis), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeMetis));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeMetis));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMetis));
    assertStringsEqual(symbol, "METIS");
    assertStringsEqual(txUrl, "https://andromeda-explorer.metis.io/tx/0x422f2ebbede32d4434ad0cf0ae55d44a84e14d3d5725a760133255b42676d8ce");
    assertStringsEqual(accUrl, "https://andromeda-explorer.metis.io/address/0xBe9E8Ec25866B21bA34e97b9393BCabBcB4A5C86");
    assertStringsEqual(id, "metis");
    assertStringsEqual(name, "Metis");
}
