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


TEST(TWSteemCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSteem, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSteem));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSteem));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSteem), 3);
    ASSERT_EQ(TWBlockchainSteem, TWCoinTypeBlockchain(TWCoinTypeSteem));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSteem));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSteem));
    assertStringsEqual(symbol, "STEEM");
    assertStringsEqual(txUrl, "https://steemblockexplorer.com/tx/123");
    assertStringsEqual(id, "steem");
    assertStringsEqual(name, "Steem");
}
