// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWMantaPacificCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMantaPacific;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x2bbd5d85b0ed05d1416e30ce1197a6f0c27d10ce02593a2719e2baf486d2e8c2"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xF122a1aC569a36a5Cf6d0F828A22254c8A9afF84"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "manta");
    assertStringsEqual(name, "Manta Pacific");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://pacific-explorer.manta.network/tx/0x2bbd5d85b0ed05d1416e30ce1197a6f0c27d10ce02593a2719e2baf486d2e8c2");
    assertStringsEqual(accUrl, "https://pacific-explorer.manta.network/address/0xF122a1aC569a36a5Cf6d0F828A22254c8A9afF84");
}
