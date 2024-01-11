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


TEST(TWOntologyCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOntology));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOntology, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOntology, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOntology));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOntology));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOntology), 0);
    ASSERT_EQ(TWBlockchainOntology, TWCoinTypeBlockchain(TWCoinTypeOntology));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOntology));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOntology));
    assertStringsEqual(symbol, "ONT");
    assertStringsEqual(txUrl, "https://explorer.ont.io/transaction/t123");
    assertStringsEqual(accUrl, "https://explorer.ont.io/address/a12");
    assertStringsEqual(id, "ontology");
    assertStringsEqual(name, "Ontology");
}
