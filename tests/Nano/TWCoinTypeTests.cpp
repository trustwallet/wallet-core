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


TEST(TWNanoCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    auto txId = TWStringCreateWithUTF8Bytes("C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNano, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNano));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNano));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNano), 30);
    ASSERT_EQ(TWBlockchainNano, TWCoinTypeBlockchain(TWCoinTypeNano));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNano));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNano));
    assertStringsEqual(symbol, "NANO");
    assertStringsEqual(txUrl, "https://nanocrawler.cc/explorer/block/C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F");
    assertStringsEqual(id, "nano");
    assertStringsEqual(name, "Nano");
}
