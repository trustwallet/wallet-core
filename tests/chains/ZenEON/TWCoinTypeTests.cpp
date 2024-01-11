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


TEST(TWZenEONCoinType, TWCoinType) {
    const auto coin = TWCoinTypeZenEON;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb462e3dac8eef21957d3b6cff3c184d083434367a726dd871e98a774f4d037a5"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x09bCfC348101B1179BCF3837aC996cF09357215f"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zeneon");
    assertStringsEqual(name, "Zen EON");
    assertStringsEqual(symbol, "ZEN");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "7332");
    assertStringsEqual(txUrl, "https://eon-explorer.horizenlabs.io/tx/0xb462e3dac8eef21957d3b6cff3c184d083434367a726dd871e98a774f4d037a5");
    assertStringsEqual(accUrl, "https://eon-explorer.horizenlabs.io/address/0x09bCfC348101B1179BCF3837aC996cF09357215f");
}
