// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWZkLinkNovaCoinType, TWCoinType) {
    const auto coin = TWCoinTypeZkLinkNova;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xeb5eb8710369c89115a83f3e744c15c9d388030cfce2fd3a653dbd18f2947400"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xF95115BaD9a4585B3C5e2bfB50579f17163A45aA"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zklinknova");
    assertStringsEqual(name, "zkLink Nova Mainnet");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.zklink.io/tx/0xeb5eb8710369c89115a83f3e744c15c9d388030cfce2fd3a653dbd18f2947400");
    assertStringsEqual(accUrl, "https://explorer.zklink.io/address/0xF95115BaD9a4585B3C5e2bfB50579f17163A45aA");
}
