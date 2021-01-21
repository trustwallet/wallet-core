// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWFlowCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFlow));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xd6cd9245032fbc4ec8e4d44ead67a3e2d3f5e7cd4cfb895d2ff1c167d3bc58a7"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFlow, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x6a7457b796dee457"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFlow, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFlow));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFlow));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFlow), 18);
    ASSERT_EQ(TWBlockchainFlow, TWCoinTypeBlockchain(TWCoinTypeFlow));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeFlow));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFlow));
    assertStringsEqual(symbol, "FLOW");
    assertStringsEqual(txUrl, "https://flow.bigdipper.live//tx/0xd6cd9245032fbc4ec8e4d44ead67a3e2d3f5e7cd4cfb895d2ff1c167d3bc58a7");
    assertStringsEqual(accUrl, "https://flow.bigdipper.live//account/0x6a7457b796dee457");
    assertStringsEqual(id, "flow");
    assertStringsEqual(name, "Flow");
}
