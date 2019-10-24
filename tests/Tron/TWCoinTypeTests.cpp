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


TEST(TWTronCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);
    ASSERT_EQ(TWBlockchainTron, TWCoinTypeBlockchain(TWCoinTypeTron));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTron));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTron));
    assertStringsEqual(symbol, "TRX");
    assertStringsEqual(txUrl, "https://tronscan.org/#/transaction/123");
    assertStringsEqual(id, "tron");
    assertStringsEqual(name, "Tron");
}
