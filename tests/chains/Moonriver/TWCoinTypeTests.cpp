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


TEST(TWMoonriverCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMoonriver;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x2e2daa3943ba65d9bbb910a4f6765aa6a466a0ef8935090547ca9d30e201e032"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x899831D937937d011305E73EE782cce0455DF15a"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "moonriver");
    assertStringsEqual(name, "Moonriver");
    assertStringsEqual(symbol, "MOVR");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1285");
    assertStringsEqual(txUrl, "https://moonriver.moonscan.io/tx/0x2e2daa3943ba65d9bbb910a4f6765aa6a466a0ef8935090547ca9d30e201e032");
    assertStringsEqual(accUrl, "https://moonriver.moonscan.io/address/0x899831D937937d011305E73EE782cce0455DF15a");
}
