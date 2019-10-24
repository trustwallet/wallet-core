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


TEST(TWARKCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeARK));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeARK, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeARK));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeARK));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeARK), 8);
    ASSERT_EQ(TWBlockchainArk, TWCoinTypeBlockchain(TWCoinTypeARK));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeARK));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeARK));
    assertStringsEqual(symbol, "ARK");
    assertStringsEqual(txUrl, "https://explorer.ark.io/transaction/123");
    assertStringsEqual(id, "ark");
    assertStringsEqual(name, "ARK");
}
