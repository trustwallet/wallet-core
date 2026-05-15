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


TEST(TWKusamaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKusama));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x834220074151531626b34acf4568cd5763a47b2eb2647bd0f0b7e46f980e0c50"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKusama, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("FfnS7Vk2i19AdFMf6D8ZRVUhNsYWVtsLmqQEnAmTqQBqsSy"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeKusama, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKusama));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKusama));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKusama), 12);
    ASSERT_EQ(TWBlockchainKusama, TWCoinTypeBlockchain(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKusama));
    assertStringsEqual(symbol, "KSM");
    assertStringsEqual(txUrl, "https://assethub-kusama.subscan.io/extrinsic/0x834220074151531626b34acf4568cd5763a47b2eb2647bd0f0b7e46f980e0c50");
    assertStringsEqual(accUrl, "https://assethub-kusama.subscan.io/account/FfnS7Vk2i19AdFMf6D8ZRVUhNsYWVtsLmqQEnAmTqQBqsSy");
    assertStringsEqual(id, "kusama");
    assertStringsEqual(name, "Kusama");
}
