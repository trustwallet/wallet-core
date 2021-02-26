// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWMaticCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMatic));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xe26ed1470d5bf99a53d687843e7acdf7e4ba6620af93b4d672e714de90476e8e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMatic, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x720E1fa107A1Df39Db4E78A3633121ac36Bec132"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMatic, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMatic));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMatic));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMatic), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeMatic));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeMatic));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMatic));
    assertStringsEqual(symbol, "MATIC");
    assertStringsEqual(txUrl, "https://explorer.matic.network//tx/0xe26ed1470d5bf99a53d687843e7acdf7e4ba6620af93b4d672e714de90476e8e");
    assertStringsEqual(accUrl, "https://explorer.matic.network//address/0x720E1fa107A1Df39Db4E78A3633121ac36Bec132");
    assertStringsEqual(id, "matic");
    assertStringsEqual(name, "Matic");
}
