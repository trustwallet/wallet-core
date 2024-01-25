// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWNativeZetaChainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeNativeZetaChain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("2DBB071DDD47985F4470A21E5943CE95D371AE4BDE2267E201D3553FB2BDCFDE"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zetachain");
    assertStringsEqual(name, "NativeZetaChain");
    assertStringsEqual(symbol, "ZETA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.zetachain.com/cosmos/tx/2DBB071DDD47985F4470A21E5943CE95D371AE4BDE2267E201D3553FB2BDCFDE");
    assertStringsEqual(accUrl, "https://explorer.zetachain.com/address/zeta14py36sx57ud82t9yrks9z6hdsrpn5x6kmxs0ne");
}
