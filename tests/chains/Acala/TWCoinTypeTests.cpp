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


TEST(TWAcalaCoinType, TWCoinType) {
    const auto coin = TWCoinTypeAcala;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xf3d58aafb1208bc09d10ba74bbf1c7811dc55a9149c1505256b6fb5603f5047f"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("26JqMKx4HJJcmb1kXo24HYYobiK2jURGCq6zuEzFBK3hQ9Ti"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "acala");
    assertStringsEqual(name, "Acala");
    assertStringsEqual(symbol, "ACA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 12);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainPolkadot);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://acala.subscan.io/extrinsic/0xf3d58aafb1208bc09d10ba74bbf1c7811dc55a9149c1505256b6fb5603f5047f");
    assertStringsEqual(accUrl, "https://acala.subscan.io/account/26JqMKx4HJJcmb1kXo24HYYobiK2jURGCq6zuEzFBK3hQ9Ti");
}
