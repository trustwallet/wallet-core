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


TEST(TWKavaEvmCoinType, TWCoinType) {
    const auto coin = TWCoinTypeKavaEvm;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xa999bd5183568ba178795e6a9d1561566fbf4a9ccc813cc475168832bc4909b3"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xF92d3DB0d9f912f285b1ec69578A6201A78487d7"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "kavaevm");
    assertStringsEqual(name, "KavaEvm");
    assertStringsEqual(symbol, "KAVA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "2222");
    assertStringsEqual(txUrl, "https://explorer.kava.io/tx/0xa999bd5183568ba178795e6a9d1561566fbf4a9ccc813cc475168832bc4909b3");
    assertStringsEqual(accUrl, "https://explorer.kava.io/address/0xF92d3DB0d9f912f285b1ec69578A6201A78487d7");
}
