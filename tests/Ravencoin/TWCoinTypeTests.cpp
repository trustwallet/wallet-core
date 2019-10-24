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


TEST(TWRavencoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRavencoin));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRavencoin, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRavencoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRavencoin));

    assertStringsEqual(symbol, "RVN");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRavencoin), 8);
    assertStringsEqual(txUrl, "https://ravencoin.network/tx/123");
    assertStringsEqual(id, "ravencoin");
    assertStringsEqual(name, "Ravencoin");
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeRavencoin));
    ASSERT_EQ(0x7a, TWCoinTypeP2shPrefix(TWCoinTypeRavencoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeRavencoin));
}
