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
    TEST(TWNobleCoinType, TWCoinType) {
        const auto coin = TWCoinTypeNoble;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("EA231079975A058FEC28EF372B445763918C098DE033E868E2E035F3F98C59C7"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("noble1y2egevq0nyzm7w6a9kpxkw86eqytcvxpwsp6d9"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "noble");
        assertStringsEqual(name, "Noble");
        assertStringsEqual(symbol, "USDC");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "noble-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/noble/txs/EA231079975A058FEC28EF372B445763918C098DE033E868E2E035F3F98C59C7");
        assertStringsEqual(accUrl, "https://www.mintscan.io/noble/account/noble1y2egevq0nyzm7w6a9kpxkw86eqytcvxpwsp6d9");
    }
}
