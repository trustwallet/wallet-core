// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWBounceBitCoinType, TWCoinType) {
    const auto coin = TWCoinTypeBounceBit;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x52558f4143d058d942e3b73414090266ae3ffce1fe8c25fe86896e2c8e5ef932"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xf4aa7349a9ccca4609943955b5ddc7bd9278c223"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "bouncebit");
    assertStringsEqual(name, "BounceBit");
    assertStringsEqual(symbol, "BB");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://bbscan.io/tx/0x52558f4143d058d942e3b73414090266ae3ffce1fe8c25fe86896e2c8e5ef932");
    assertStringsEqual(accUrl, "https://bbscan.io/address/0xf4aa7349a9ccca4609943955b5ddc7bd9278c223");
}
