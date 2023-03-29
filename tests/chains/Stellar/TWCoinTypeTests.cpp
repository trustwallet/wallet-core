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


TEST(TWStellarCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("d9aeabfa9d24df8c5755125f8af243b74cd3ff878656cfa72c566a8824bf6e84"));
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
    assertStringsEqual(txUrl, "https://blockchair.com/stellar/transaction/d9aeabfa9d24df8c5755125f8af243b74cd3ff878656cfa72c566a8824bf6e84");
    assertStringsEqual(accUrl, "https://blockchair.com/stellar/account/GCILJZQ3CKBKBUJWW4TAM6Q37LJA5MQX6GMSFSQN75BPLWIZ33OPRG52");
    assertStringsEqual(id, "stellar");
    assertStringsEqual(name, "Stellar");
}
