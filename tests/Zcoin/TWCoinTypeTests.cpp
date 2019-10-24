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


TEST(TWZcoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcoin));
    ASSERT_EQ(0x7, TWCoinTypeP2shPrefix(TWCoinTypeZcoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeZcoin));
    assertStringsEqual(symbol, "XZC");
    assertStringsEqual(txUrl, "https://explorer.zcoin.io/tx/123");
    assertStringsEqual(id, "zcoin");
    assertStringsEqual(name, "Zcoin");
}
