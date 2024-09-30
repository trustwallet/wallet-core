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


TEST(TWNanoCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNano, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("nano_1wpj616kwhe1y38y1mspd8aub8i334cwybqco511iyuxm55zx8d67ptf1tsf"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNano, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNano));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNano));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNano), 30);
    ASSERT_EQ(TWBlockchainNano, TWCoinTypeBlockchain(TWCoinTypeNano));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNano));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNano));
    assertStringsEqual(symbol, "XNO");
    assertStringsEqual(txUrl, "https://www.nanolooker.com/block/C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F");
    assertStringsEqual(accUrl, "https://www.nanolooker.com/account/nano_1wpj616kwhe1y38y1mspd8aub8i334cwybqco511iyuxm55zx8d67ptf1tsf");
    assertStringsEqual(id, "nano");
    assertStringsEqual(name, "Nano");
}
