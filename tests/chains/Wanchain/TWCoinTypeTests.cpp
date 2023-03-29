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


TEST(TWWanchainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWanchain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x180ea96a3218b82b9b35d796823266d8a425c182507adfe5eeffc96e6a14d856"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x69B492D57bb777e97aa7044D0575228434e2E8B1"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeWanchain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWanchain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWanchain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanchain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeWanchain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeWanchain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeWanchain));
    assertStringsEqual(symbol, "WAN");
    assertStringsEqual(txUrl, "https://www.wanscan.org/tx/0x180ea96a3218b82b9b35d796823266d8a425c182507adfe5eeffc96e6a14d856");
    assertStringsEqual(accUrl, "https://www.wanscan.org/address/0x69B492D57bb777e97aa7044D0575228434e2E8B1");
    assertStringsEqual(id, "wanchain");
    assertStringsEqual(name, "Wanchain");
}
