// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWOsmosisCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOsmosis));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("5A6E50A6F2927E4B8C87BB094D5FBF15F1287429A09111806FC44B3CD86CACA8"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOsmosis, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOsmosis, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOsmosis));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOsmosis));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOsmosis), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeOsmosis));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOsmosis));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOsmosis));
    assertStringsEqual(symbol, "OSMO");
    assertStringsEqual(txUrl, "https://mintscan.io/osmosis/txs/5A6E50A6F2927E4B8C87BB094D5FBF15F1287429A09111806FC44B3CD86CACA8");
    assertStringsEqual(accUrl, "https://mintscan.io/osmosis/account/osmo1mky69cn8ektwy0845vec9upsdphktxt0en97f5");
    assertStringsEqual(id, "osmosis");
    assertStringsEqual(name, "Osmosis");
}
