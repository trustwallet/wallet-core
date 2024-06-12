// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWLightlinkCoinType, TWCoinType) {
    const auto coin = TWCoinTypeLightlink;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xc65f82445aefc883fd4ffe09149c8ce48f61b670c0734528a49d4a8bd8309bb0"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4566ED6c7a7fFc90E2C7cfF7eB9156262afD2fDe"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "lightlink");
    assertStringsEqual(name, "Lightlink Phoenix");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://phoenix.lightlink.io/tx/0xc65f82445aefc883fd4ffe09149c8ce48f61b670c0734528a49d4a8bd8309bb0");
    assertStringsEqual(accUrl, "https://phoenix.lightlink.io/address/0x4566ED6c7a7fFc90E2C7cfF7eB9156262afD2fDe");
}
