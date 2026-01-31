// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWXGRCoinType, TWCoinType) {
    const auto coin = TWCoinTypeXGR;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xe7c2a71943e7c79b404b473c1f595adaf1c5b3896102197a74f63205ea5dfa80"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x2A021a1B25DA25e14C4046e5BAc9375Ec3bebf8c"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "xgr");
    assertStringsEqual(name, "XGR");
    assertStringsEqual(symbol, "XGR");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.xgr.network/tx/0xe7c2a71943e7c79b404b473c1f595adaf1c5b3896102197a74f63205ea5dfa80");
    assertStringsEqual(accUrl, "https://explorer.xgr.network/address/0x2A021a1B25DA25e14C4046e5BAc9375Ec3bebf8c");
}
