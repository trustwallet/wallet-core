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


TEST(TWMultiversXCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMultiversX));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("163b46551a74626415074b626d2f37d3c78aef0f6ccb628db434ee65a35ea127"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMultiversX, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMultiversX, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMultiversX));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMultiversX));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMultiversX), 18);
    ASSERT_EQ(TWBlockchainMultiversX, TWCoinTypeBlockchain(TWCoinTypeMultiversX));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeMultiversX));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMultiversX));
    assertStringsEqual(symbol, "eGLD");
    assertStringsEqual(txUrl, "https://explorer.multiversx.com/transactions/163b46551a74626415074b626d2f37d3c78aef0f6ccb628db434ee65a35ea127");
    assertStringsEqual(accUrl, "https://explorer.multiversx.com/accounts/erd1qyu5wthldzr8wx5c9ucg8kjagg0jfs53s8nr3zpz3hypefsdd8ssycr6th");
    assertStringsEqual(id, "elrond");
    assertStringsEqual(name, "MultiversX");
}
