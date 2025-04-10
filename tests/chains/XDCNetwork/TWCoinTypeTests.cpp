// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWXDCNetworkCoinType, TWCoinType) {
    const auto coin = TWCoinTypeXDCNetwork;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xeb7f118ef101ed5a1505db73163e806801f21496a399e6e8e1f99e553589df3f"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x22e1DCFF9Bba3B751b8bACF0d9c25eE3D9110613"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "xdc");
    assertStringsEqual(name, "XDC Network");
    assertStringsEqual(symbol, "XDC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://xdcscan.com/tx/0xeb7f118ef101ed5a1505db73163e806801f21496a399e6e8e1f99e553589df3f");
    assertStringsEqual(accUrl, "https://xdcscan.com/address/0x22e1DCFF9Bba3B751b8bACF0d9c25eE3D9110613");
}
