// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWMonadCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMonad;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x8394f9f01bc2ae2cc93e19170bf80c303210f6f4198e5ec3cc99b0cba04962b6"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x6ab69B482987b0BA1f1c96BDbDC192a80CB09132"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "monad");
    assertStringsEqual(name, "Monad");
    assertStringsEqual(symbol, "MON");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://monvision.io/tx/0x8394f9f01bc2ae2cc93e19170bf80c303210f6f4198e5ec3cc99b0cba04962b6");
    assertStringsEqual(accUrl, "https://monvision.io/address/0x6ab69B482987b0BA1f1c96BDbDC192a80CB09132");
}
