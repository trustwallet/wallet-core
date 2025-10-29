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
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xfffe6b1c1238be8d5f4779f97af8fffc89530e2634c61be7a5241dc08f41f07f"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xa161C1d86b26ac692604E0cDbc5EcB2D1d85759c"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "monad");
    assertStringsEqual(name, "Monad Testnet");
    assertStringsEqual(symbol, "MON");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://testnet.monadexplorer.com/tx/0xfffe6b1c1238be8d5f4779f97af8fffc89530e2634c61be7a5241dc08f41f07f");
    assertStringsEqual(accUrl, "https://testnet.monadexplorer.com/address/0xa161C1d86b26ac692604E0cDbc5EcB2D1d85759c");
}
