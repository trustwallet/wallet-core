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


TEST(TWPolygonCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePolygon));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xe26ed1470d5bf99a53d687843e7acdf7e4ba6620af93b4d672e714de90476e8e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePolygon, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x720E1fa107A1Df39Db4E78A3633121ac36Bec132"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePolygon, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePolygon));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePolygon));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePolygon), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePolygon));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePolygon));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePolygon));
    assertStringsEqual(symbol, "MATIC");
    assertStringsEqual(txUrl, "https://polygonscan.com/tx/0xe26ed1470d5bf99a53d687843e7acdf7e4ba6620af93b4d672e714de90476e8e");
    assertStringsEqual(accUrl, "https://polygonscan.com/address/0x720E1fa107A1Df39Db4E78A3633121ac36Bec132");
    assertStringsEqual(id, "polygon");
    assertStringsEqual(name, "Polygon");
}
