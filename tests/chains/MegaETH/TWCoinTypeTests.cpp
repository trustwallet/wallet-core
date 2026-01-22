// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWMegaETHCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMegaETH;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x9021590996bda7682485fff4ab1ed221cea2fa0b352073f69e5a76114c7ed81a"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x6a4D1b2fC6a894928979F152A8c9245D96860604"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "megaeth");
    assertStringsEqual(name, "MegaETH");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://megaeth.blockscout.com/tx/0x9021590996bda7682485fff4ab1ed221cea2fa0b352073f69e5a76114c7ed81a");
    assertStringsEqual(accUrl, "https://megaeth.blockscout.com/address/0x6a4D1b2fC6a894928979F152A8c9245D96860604");
}
