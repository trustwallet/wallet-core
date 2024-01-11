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

namespace TW::Cosmos::tests {
    TEST(TWPersistenceCoinType, TWCoinType) {
        const auto coin = TWCoinTypePersistence;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("BBD9DEE03A8D7538D8E7398217467F4A2B5690D15773E8A6442E6AEEEFA21E64"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("persistence10ys69560pqr6zmqam80g8s0smtjw6p3ugzmy3u"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "persistence");
        assertStringsEqual(name, "Persistence");
        assertStringsEqual(symbol, "XPRT");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "core-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/persistence/txs/BBD9DEE03A8D7538D8E7398217467F4A2B5690D15773E8A6442E6AEEEFA21E64");
        assertStringsEqual(accUrl, "https://www.mintscan.io/persistence/account/persistence10ys69560pqr6zmqam80g8s0smtjw6p3ugzmy3u");
    }
}
