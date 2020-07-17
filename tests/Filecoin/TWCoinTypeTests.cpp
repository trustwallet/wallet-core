// Copyright © 2017-2020 Trust Wallet.
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

TEST(TWFilecoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFilecoin));
    auto txId = TWStringCreateWithUTF8Bytes(
        "bafy2bzacecbm3ofxjjzcl2rg32ninphza34mm3ijr55zjsamwfqmz4ib63mqe");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFilecoin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("t1nbb73vhk5dtmnsgeaetbo76daepqjtrfoccn74i");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFilecoin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFilecoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFilecoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFilecoin), 18);
    ASSERT_EQ(TWBlockchainFilecoin, TWCoinTypeBlockchain(TWCoinTypeFilecoin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeFilecoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFilecoin));
    assertStringsEqual(symbol, "FIL");
    assertStringsEqual(txUrl,
                       "https://filscan.io/#/message/"
                       "detail?cid=bafy2bzacecbm3ofxjjzcl2rg32ninphza34mm3ijr55zjsamwfqmz4ib63mqe");
    assertStringsEqual(
        accUrl,
        "https://filscan.io/#/address/detail?address=t1nbb73vhk5dtmnsgeaetbo76daepqjtrfoccn74i");
    assertStringsEqual(id, "filecoin");
    assertStringsEqual(name, "Filecoin");
}
