// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWWardenProtocolCoinType, TWCoinType) {
    const auto coin = TWCoinTypeWardenProtocol;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x90aaaaf8379eecc479ac8679a1430e7fb53d2e64749c4bc9132371d4e1ea98f2"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xDc3D9d2f5fC5bEb330c7135b2c38f354c18656f0"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "wardenprotocol");
    assertStringsEqual(name, "Warden Protocol");
    assertStringsEqual(symbol, "WARD");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://explorer.wardenprotocol.org/tx/0x90aaaaf8379eecc479ac8679a1430e7fb53d2e64749c4bc9132371d4e1ea98f2");
    assertStringsEqual(accUrl, "https://explorer.wardenprotocol.org/address/0xDc3D9d2f5fC5bEb330c7135b2c38f354c18656f0");
}
