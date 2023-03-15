// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWTezosCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("onk3Z6V4StyfiXTPSHwZFvTKVAaws37cHmZacmULPr3VbVHpKrg"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("tz1SiPXX4MYGNJNDsRc7n8hkvUqFzg8xqF9m"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTezos, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);
    ASSERT_EQ(TWBlockchainTezos, TWCoinTypeBlockchain(TWCoinTypeTezos));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTezos));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTezos));
    assertStringsEqual(symbol, "XTZ");
    assertStringsEqual(txUrl, "https://tzstats.com/onk3Z6V4StyfiXTPSHwZFvTKVAaws37cHmZacmULPr3VbVHpKrg");
    assertStringsEqual(accUrl, "https://tzstats.com/tz1SiPXX4MYGNJNDsRc7n8hkvUqFzg8xqF9m");
    assertStringsEqual(id, "tezos");
    assertStringsEqual(name, "Tezos");
}
