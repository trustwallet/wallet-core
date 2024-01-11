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


TEST(TWSecretCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSecret;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("026B4886B1D9CE836A99755DDE99D4F8A7748D27B1CE9D298A763B1CFFF62C00"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("secret167m3s89ddurjpyr82vsluvvj0t8ylzn95trrqy"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "secret");
    assertStringsEqual(name, "Secret");
    assertStringsEqual(symbol, "SCRT");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "secret-4");
    assertStringsEqual(txUrl, "https://mintscan.io/secret/txs/026B4886B1D9CE836A99755DDE99D4F8A7748D27B1CE9D298A763B1CFFF62C00");
    assertStringsEqual(accUrl, "https://mintscan.io/secret/account/secret167m3s89ddurjpyr82vsluvvj0t8ylzn95trrqy");
}
