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


TEST(TWSuiCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSui;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("68wBKsZyYXmCUydDmabQ71kTcFWTfDG7tFmTLk1HgNdN"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "sui");
    assertStringsEqual(name, "Sui");
    assertStringsEqual(symbol, "SUI");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 9);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainSui);
    assertStringsEqual(txUrl, "https://suiscan.xyz/mainnet/tx/68wBKsZyYXmCUydDmabQ71kTcFWTfDG7tFmTLk1HgNdN");
    assertStringsEqual(accUrl, "https://suiscan.xyz/mainnet/account/0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2");
}
