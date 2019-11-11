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


TEST(TWSampleCoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSampleCoin));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSampleCoin, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSampleCoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSampleCoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSampleCoin), 18);
    ASSERT_EQ(TWBlockchainSample, TWCoinTypeBlockchain(TWCoinTypeSampleCoin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSampleCoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSampleCoin));
    assertStringsEqual(symbol, "SPL");
    assertStringsEqual(txUrl, "https://etherscan.io/tx/123");
    assertStringsEqual(id, "sample");
    assertStringsEqual(name, "SampleCoin");
}
