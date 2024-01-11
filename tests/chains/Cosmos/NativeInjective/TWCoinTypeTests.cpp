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


TEST(TWNativeInjectiveCoinType, TWCoinType) {
    const auto coin = TWCoinTypeNativeInjective;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("C5F6A4FF9DF1AE9FF543D2CEBD8E3E9B04290B2445F9D91D7707EDBF4B7EE16B"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "nativeinjective");
    assertStringsEqual(name, "Native Injective");
    assertStringsEqual(symbol, "INJ");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainNativeInjective);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "injective-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/injective/txs/C5F6A4FF9DF1AE9FF543D2CEBD8E3E9B04290B2445F9D91D7707EDBF4B7EE16B");
    assertStringsEqual(accUrl, "https://www.mintscan.io/injective/account/inj1xmpkmxr4as00em23tc2zgmuyy2gr4h3wgcl6vd");
}
