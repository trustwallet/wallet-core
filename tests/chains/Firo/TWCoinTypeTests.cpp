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


TEST(TWFiroCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFiro));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("09a60d58b3d17519a42a8eca60750c33b710ca8f3ca71994192e05c248a2a111"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFiro, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a8ULhhDgfdSiXJhSZVdhb8EuDc6R3ogsaM"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFiro, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFiro));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFiro));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFiro), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeFiro));
    ASSERT_EQ(0x7, TWCoinTypeP2shPrefix(TWCoinTypeFiro));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFiro));
    assertStringsEqual(symbol, "FIRO");
    assertStringsEqual(txUrl, "https://explorer.firo.org/tx/09a60d58b3d17519a42a8eca60750c33b710ca8f3ca71994192e05c248a2a111");
    assertStringsEqual(accUrl, "https://explorer.firo.org/address/a8ULhhDgfdSiXJhSZVdhb8EuDc6R3ogsaM");
    assertStringsEqual(id, "firo");
    assertStringsEqual(name, "Firo");
}
