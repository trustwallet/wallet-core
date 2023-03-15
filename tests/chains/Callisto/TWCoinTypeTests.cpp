// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWCallistoCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCallisto, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCallisto));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCallisto));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCallisto));
    assertStringsEqual(symbol, "CLO");
    assertStringsEqual(txUrl, "https://explorer.callisto.network/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.callisto.network/addr/a12");
    assertStringsEqual(id, "callisto");
    assertStringsEqual(name, "Callisto");
}
