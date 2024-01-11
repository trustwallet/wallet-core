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


TEST(TWGreenfieldCoinType, TWCoinType) {
    const auto coin = TWCoinTypeGreenfield;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x150eac42070957115fd538b1f348fadd78d710fb641c248120efcf35d1e7e4f3"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xcf0f6b88ed72653b00fdebbffc90b98072cb3285"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "greenfield");
    assertStringsEqual(name, "BNB Greenfield");
    assertStringsEqual(symbol, "BNB");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainGreenfield);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1017");
    assertStringsEqual(txUrl, "https://greenfieldscan.com/tx/0x150eac42070957115fd538b1f348fadd78d710fb641c248120efcf35d1e7e4f3");
    assertStringsEqual(accUrl, "https://greenfieldscan.com/account/0xcf0f6b88ed72653b00fdebbffc90b98072cb3285");
}
