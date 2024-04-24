// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TW{COIN_TYPE}CoinType, TWCoinType) {
    const auto coin = TWCoinType{COIN_TYPE};
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("{EXPLORER_SAMPLE_TX}"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("{EXPLORER_SAMPLE_ACCOUNT}"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "{COIN_ID}");
    assertStringsEqual(name, "{COIN_NAME}");
    assertStringsEqual(symbol, "{SYMBOL}");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), {DECIMALS});
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchain{BLOCKCHAIN});
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), {P2PKH_PREFIX});
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), {P2SH_PREFIX});
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), {STATIC_PREFIX});
    assertStringsEqual(txUrl, "{EXPLORER_URL}{EXPLORER_TX_PATH}{EXPLORER_SAMPLE_TX}");
    assertStringsEqual(accUrl, "{EXPLORER_URL}{EXPLORER_ACCOUNT_PATH}{EXPLORER_SAMPLE_ACCOUNT}");
}
