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


TEST(TWStellarCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("8a7ff7261e8b3f31af7f6ed257c2e9fe7c47afcd9b1ce1be1bfc1bc5f6a3ad9e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeStellar, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("GCILJZQ3CKBKBUJWW4TAM6Q37LJA5MQX6GMSFSQN75BPLWIZ33OPRG52"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeStellar, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeStellar));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeStellar));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeStellar), 7);
    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeStellar));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeStellar));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeStellar));
    assertStringsEqual(symbol, "XLM");
    assertStringsEqual(txUrl, "https://blockchair.com/stellar/transaction/8a7ff7261e8b3f31af7f6ed257c2e9fe7c47afcd9b1ce1be1bfc1bc5f6a3ad9e");
    assertStringsEqual(accUrl, "https://blockchair.com/stellar/account/GCILJZQ3CKBKBUJWW4TAM6Q37LJA5MQX6GMSFSQN75BPLWIZ33OPRG52");
    assertStringsEqual(id, "stellar");
    assertStringsEqual(name, "Stellar");
}
