// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWPlasmaCoinType, TWCoinType) {
    const auto coin = TWCoinTypePlasma;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x3700212ec535b4c804363be87ba8a5f5668de6314ed41978f6ad71c5340d4d77"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x30A3E1F27B60c095E2c87bce1e2ffB24f381C8cd"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "plasma");
    assertStringsEqual(name, "Plasma Mainnet");
    assertStringsEqual(symbol, "XPL");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://plasmascan.to/tx/0x3700212ec535b4c804363be87ba8a5f5668de6314ed41978f6ad71c5340d4d77");
    assertStringsEqual(accUrl, "https://plasmascan.to/address/0x30A3E1F27B60c095E2c87bce1e2ffB24f381C8cd");
}
