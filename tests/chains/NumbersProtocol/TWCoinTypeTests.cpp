// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWNumbersProtocolCoinType, TWCoinType) {
    const auto coin = TWCoinTypeNumbersProtocol;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x3cc633cf6bfbde82ec65a6458524a82929810f69e1c0b711d971c1a991226a7e"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x2FBfE8F2bA00B255e60c220755040B597d09aFFa"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "num");
    assertStringsEqual(name, "Numbers Protocol");
    assertStringsEqual(symbol, "NUM");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://mainnet.num.network/tx/0x3cc633cf6bfbde82ec65a6458524a82929810f69e1c0b711d971c1a991226a7e");
    assertStringsEqual(accUrl, "https://mainnet.num.network/address/0x2FBfE8F2bA00B255e60c220755040B597d09aFFa");
}
