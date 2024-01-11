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


TEST(TWZelcashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZelcash));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZelcash, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZelcash, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZelcash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZelcash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZelcash), 8);
    ASSERT_EQ(TWBlockchainZcash, TWCoinTypeBlockchain(TWCoinTypeZelcash));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZelcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZelcash));
    assertStringsEqual(symbol, "FLUX");
    assertStringsEqual(txUrl, "https://explorer.runonflux.io/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.runonflux.io/address/a12");
    assertStringsEqual(id, "zelcash");
    assertStringsEqual(name, "Flux");
}
