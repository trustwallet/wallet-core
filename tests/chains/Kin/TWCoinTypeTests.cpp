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


TEST(TWKinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeKin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKin), 5);
    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeKin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKin));
    assertStringsEqual(symbol, "KIN");
    assertStringsEqual(txUrl, "https://www.kin.org/blockchainInfoPage/?&dataType=public&header=Transaction&id=t123");
    assertStringsEqual(accUrl, "https://www.kin.org/blockchainAccount/?&dataType=public&header=accountID&id=a12");
    assertStringsEqual(id, "kin");
    assertStringsEqual(name, "Kin");
}
