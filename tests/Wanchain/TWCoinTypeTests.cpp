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


TEST(TWWanchainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWanchain));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWanchain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWanchain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanchain), 18);
    ASSERT_EQ(TWBlockchainWanchain, TWCoinTypeBlockchain(TWCoinTypeWanchain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeWanchain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeWanchain));
    assertStringsEqual(symbol, "WAN");
    assertStringsEqual(txUrl, "https://www.wanscan.org/tx/123");
    assertStringsEqual(id, "wanchain");
    assertStringsEqual(name, "Wanchain");
}
