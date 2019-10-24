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


TEST(TWThunderTokenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeThunderToken));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeThunderToken));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeThunderToken));
    assertStringsEqual(symbol, "TT");
    assertStringsEqual(txUrl, "https://scan.thundercore.com/transactions/123");
    assertStringsEqual(id, "thundertoken");
    assertStringsEqual(name, "Thunder Token");
}
