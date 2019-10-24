// Copyright © 2017-2019 Trust Wallet.
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


TEST(TWCallistoCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCallisto));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCallisto));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCallisto));
    assertStringsEqual(symbol, "CLO");
    assertStringsEqual(txUrl, "https://explorer2.callisto.network/tx/123");
    assertStringsEqual(id, "callisto");
    assertStringsEqual(name, "Callisto");
}
