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

TEST(TWConfluxeSpaceCoinType, TWCoinType) {
    const auto coin = TWCoinTypeConfluxeSpace;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x920efefb3213b2d6a3b84149cb50b61a813d085443a20e1b0eab74120e41ff72"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x337a087daef75c72871de592fbbba570829a936a"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "cfxevm");
    assertStringsEqual(name, "Conflux eSpace");
    assertStringsEqual(symbol, "CFX");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1030");
    assertStringsEqual(txUrl, "https://evm.confluxscan.net/tx/0x920efefb3213b2d6a3b84149cb50b61a813d085443a20e1b0eab74120e41ff72");
    assertStringsEqual(accUrl, "https://evm.confluxscan.net/address/0x337a087daef75c72871de592fbbba570829a936a");
}
