// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWHypeEVMCoinType, TWCoinType) {
    const auto coin = TWCoinTypeHypeEVM;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xc79cb0bcc08995ac78f74f4a62bfd721d50a7368979b8025f3f8c229bb5a334c"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x3c43014aedbb496e173059f94bfe3b0ed1d55ba0"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "hypeevm");
    assertStringsEqual(name, "HypeEVM");
    assertStringsEqual(symbol, "HYPE");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://hyperevmscan.io/tx/0xc79cb0bcc08995ac78f74f4a62bfd721d50a7368979b8025f3f8c229bb5a334c");
    assertStringsEqual(accUrl, "https://hyperevmscan.io/address/0x3c43014aedbb496e173059f94bfe3b0ed1d55ba0");
}
