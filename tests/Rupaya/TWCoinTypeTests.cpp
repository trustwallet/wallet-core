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


TEST(TWRupayaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRupaya));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRupaya, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeRupaya, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRupaya));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRupaya));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRupaya), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeRupaya));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeRupaya));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeRupaya));
    assertStringsEqual(symbol, "RUPX");
    assertStringsEqual(txUrl, "https://scan.rupaya.io/txs/t123");
    assertStringsEqual(accUrl, "https://scan.rupaya.io/address/a12");
    assertStringsEqual(id, "rupaya");
    assertStringsEqual(name, "Rupaya");
}
