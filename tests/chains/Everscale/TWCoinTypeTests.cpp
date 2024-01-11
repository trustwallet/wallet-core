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

namespace TW::Everscale {

TEST(TWEverscaleCoinType, TWCoinType) {
    const auto coin = TWCoinTypeEverscale;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("781238b2b0d15cd4cd2e2a0a142753750cd5e1b2c8b506fcede75a90e02f1268"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0:d2bf59964a05dee84a0dd1ddc0ad83ba44d49719cf843d689dc8b726d0fb59d8"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "everscale");
    assertStringsEqual(name, "Everscale");
    assertStringsEqual(symbol, "EVER");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 9);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEverscale);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://everscan.io/transactions/781238b2b0d15cd4cd2e2a0a142753750cd5e1b2c8b506fcede75a90e02f1268");
    assertStringsEqual(accUrl, "https://everscan.io/accounts/0:d2bf59964a05dee84a0dd1ddc0ad83ba44d49719cf843d689dc8b726d0fb59d8");
}

} // namespace TW::Everscale
