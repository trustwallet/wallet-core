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

namespace TW::Cosmos::tests {
    TEST(TWCoreumCoinType, TWCoinType) {
        const auto coin = TWCoinTypeCoreum;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("32A4AE2AE6AAE31E75EDDADE0AB9F1499ABD5AD8D3F261ADEF2805CD46FF74E7"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("core1zmwdnfpwuymwn0fkwnj2aaje34npd5sqgjxq9v"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "coreum");
        assertStringsEqual(name, "Coreum");
        assertStringsEqual(symbol, "CORE");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "coreum-mainnet-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/coreum/txs/32A4AE2AE6AAE31E75EDDADE0AB9F1499ABD5AD8D3F261ADEF2805CD46FF74E7");
        assertStringsEqual(accUrl, "https://www.mintscan.io/coreum/account/core1zmwdnfpwuymwn0fkwnj2aaje34npd5sqgjxq9v");
    }
}
