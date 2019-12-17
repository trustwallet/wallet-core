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


TEST(TWBravoCoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBravoCoin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBravoCoin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBravoCoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBravoCoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBravoCoin), 3);
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeBravoCoin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBravoCoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBravoCoin));
    assertStringsEqual(symbol, "BRAVO");
    assertStringsEqual(txUrl, "https://explorer.bravocoin.com/txid/t123");
    assertStringsEqual(accUrl, "https://explorer.bravocoin.com/?a12");
    assertStringsEqual(id, "bravocoin");
    assertStringsEqual(name, "BravoCoin");
}
