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


TEST(TWBitcoinGoldCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinGold));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinGold, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinGold));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinGold));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinGold), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoinGold));
    ASSERT_EQ(23, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinGold));
    ASSERT_EQ(0, TWCoinTypeStaticPrefix(TWCoinTypeBitcoinGold));
    assertStringsEqual(symbol, "BTG");
    assertStringsEqual(txUrl, "https://explorer.bitcoingold.org/insight/tx/123");
    assertStringsEqual(id, "bitcoingold");
    assertStringsEqual(name, "Bitcoin Gold");
}
