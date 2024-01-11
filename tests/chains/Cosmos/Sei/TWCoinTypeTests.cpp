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
    TEST(TWSeiCoinType, TWCoinType) {
        const auto coin = TWCoinTypeSei;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("4A2114EE45317439690F3BEA9C8B6CFA11D42CF978F9487754902D372EEB488C"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("sei155hqv2rsypqzq0zpjn72frsxx4l6tcmplw63m2"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "sei");
        assertStringsEqual(name, "Sei");
        assertStringsEqual(symbol, "SEI");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "pacific-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/sei/txs/4A2114EE45317439690F3BEA9C8B6CFA11D42CF978F9487754902D372EEB488C");
        assertStringsEqual(accUrl, "https://www.mintscan.io/sei/account/sei155hqv2rsypqzq0zpjn72frsxx4l6tcmplw63m2");
    }
}
