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


TEST(TWAlgorandCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAlgorand));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAlgorand, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAlgorand));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAlgorand));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAlgorand), 6);
    ASSERT_EQ(TWBlockchainAlgorand, TWCoinTypeBlockchain(TWCoinTypeAlgorand));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAlgorand));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAlgorand));
    assertStringsEqual(symbol, "ALGO");
    assertStringsEqual(txUrl, "https://algoexplorer.io/tx/123");
    assertStringsEqual(id, "algorand");
    assertStringsEqual(name, "Algorand");
}
