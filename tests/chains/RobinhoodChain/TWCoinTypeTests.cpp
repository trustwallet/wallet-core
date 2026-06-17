// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWRobinhoodChainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeRobinhoodChain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x0364950bbd89012b11a71eeb80df2f1504678836f2918dc8809fe940706c02ec"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x7943e237c7F95DA44E0301572D358911207852Fa"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "robinhoodchain");
    assertStringsEqual(name, "Robinhood Chain");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.testnet.chain.robinhood.com/tx/0x0364950bbd89012b11a71eeb80df2f1504678836f2918dc8809fe940706c02ec");
    assertStringsEqual(accUrl, "https://explorer.testnet.chain.robinhood.com/address/0x7943e237c7F95DA44E0301572D358911207852Fa");
}
