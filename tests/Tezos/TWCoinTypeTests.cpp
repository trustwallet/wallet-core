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


TEST(TWTezosCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);
    ASSERT_EQ(TWBlockchainTezos, TWCoinTypeBlockchain(TWCoinTypeTezos));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTezos));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTezos));
    assertStringsEqual(symbol, "XTZ");
    assertStringsEqual(txUrl, "https://tezos.id/123");
    assertStringsEqual(id, "tezos");
    assertStringsEqual(name, "Tezos");
}
