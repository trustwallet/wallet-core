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
    TEST(TWFetchAICoinType, TWCoinType) {
        const auto coin = TWCoinTypeFetchAI;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("7EB4F6C26809BA047F81CEFD0889775AC8522B7B8AF559B436083BE7039C5EA6"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("fetch1t3qet68dr0qkmrjtq89lrx837qa2t05265qy6s"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "fetchai");
        assertStringsEqual(name, "Fetch AI");
        assertStringsEqual(symbol, "FET");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "fetchhub-4");
        assertStringsEqual(txUrl, "https://www.mintscan.io/fetchai/txs/7EB4F6C26809BA047F81CEFD0889775AC8522B7B8AF559B436083BE7039C5EA6");
        assertStringsEqual(accUrl, "https://www.mintscan.io/fetchai/account/fetch1t3qet68dr0qkmrjtq89lrx837qa2t05265qy6s");
    }
}
