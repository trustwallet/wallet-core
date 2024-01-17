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


TEST(TWZenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZen));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("b7f548640766fb024247accf4e01bec37d88d49c4900357edc84d49a09ff4430"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZen, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("znRchPtvEyJJUwGbCALqyjwHJb1Gx6z4H4j"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZen, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZen));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZen));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZen), 8);
    ASSERT_EQ(TWBlockchainZen, TWCoinTypeBlockchain(TWCoinTypeZen));
    ASSERT_EQ(0x96, TWCoinTypeP2shPrefix(TWCoinTypeZen));
    ASSERT_EQ(0x20, TWCoinTypeStaticPrefix(TWCoinTypeZen));
    assertStringsEqual(symbol, "ZEN");
    assertStringsEqual(txUrl, "https://explorer.horizen.io/tx/b7f548640766fb024247accf4e01bec37d88d49c4900357edc84d49a09ff4430");
    assertStringsEqual(accUrl, "https://explorer.horizen.io/address/znRchPtvEyJJUwGbCALqyjwHJb1Gx6z4H4j");
    assertStringsEqual(id, "zen");
    assertStringsEqual(name, "Zen");
}
