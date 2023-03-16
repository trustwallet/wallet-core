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


TEST(TWMonacoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMonacoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMonacoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMonacoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMonacoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMonacoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMonacoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeMonacoin));
    ASSERT_EQ(0x37, TWCoinTypeP2shPrefix(TWCoinTypeMonacoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMonacoin));
    assertStringsEqual(symbol, "MONA");
    assertStringsEqual(txUrl, "https://blockbook.electrum-mona.org/tx/t123");
    assertStringsEqual(accUrl, "https://blockbook.electrum-mona.org/address/a12");
    assertStringsEqual(id, "monacoin");
    assertStringsEqual(name, "Monacoin");
}
