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


TEST(TWIOSTCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIOST));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIOST, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIOST));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIOST));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIOST), 8);
    ASSERT_EQ(TWBlockchainIOST, TWCoinTypeBlockchain(TWCoinTypeIOST));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeIOST));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeIOST));
    assertStringsEqual(symbol, "IOST");
    assertStringsEqual(txUrl, "https://www.iostabc.com/tx/123");
    assertStringsEqual(id, "iost");
    assertStringsEqual(name, "IOST");
}
