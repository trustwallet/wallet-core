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


TEST(TWKusamaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKusama));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xcbe0c2e2851c1245bedaae4d52f06eaa6b4784b786bea2f0bff11af7715973dd"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKusama, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("DbCNECPna3k6MXFWWNZa5jGsuWycqEE6zcUxZYkxhVofrFk"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeKusama, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKusama));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKusama));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKusama), 12);
    ASSERT_EQ(TWBlockchainKusama, TWCoinTypeBlockchain(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKusama));
    assertStringsEqual(symbol, "KSM");
    assertStringsEqual(txUrl, "https://kusama.subscan.io/extrinsic/0xcbe0c2e2851c1245bedaae4d52f06eaa6b4784b786bea2f0bff11af7715973dd");
    assertStringsEqual(accUrl, "https://kusama.subscan.io/account/DbCNECPna3k6MXFWWNZa5jGsuWycqEE6zcUxZYkxhVofrFk");
    assertStringsEqual(id, "kusama");
    assertStringsEqual(name, "Kusama");
}
