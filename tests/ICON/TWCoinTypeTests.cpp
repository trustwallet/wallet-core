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


TEST(TWICONCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeICON), 18);
    ASSERT_EQ(TWBlockchainIcon, TWCoinTypeBlockchain(TWCoinTypeICON));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeICON));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeICON));
    assertStringsEqual(symbol, "ICX");
    assertStringsEqual(txUrl, "https://tracker.icon.foundation/transaction/123");
    assertStringsEqual(id, "icon");
    assertStringsEqual(name, "ICON");
}
