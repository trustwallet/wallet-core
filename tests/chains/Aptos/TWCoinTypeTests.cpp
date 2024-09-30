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


TEST(TWAptosCoinType, TWCoinType) {
    const auto coin = TWCoinTypeAptos;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xedc88058e27f6c065fd6607e262cb2a83a65f74301df90c61923014c59f9d465"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x60ad80e8cdadb81399e8a738014bc9ec865cef842f7c2cf7d84fbf7e40d065"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "aptos");
    assertStringsEqual(name, "Aptos");
    assertStringsEqual(symbol, "APT");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 8);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainAptos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://explorer.aptoslabs.com/txn/0xedc88058e27f6c065fd6607e262cb2a83a65f74301df90c61923014c59f9d465");
    assertStringsEqual(accUrl, "https://explorer.aptoslabs.com/account/0x60ad80e8cdadb81399e8a738014bc9ec865cef842f7c2cf7d84fbf7e40d065");
}
