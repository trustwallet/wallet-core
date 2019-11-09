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


TEST(TWLitecoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLitecoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeLitecoin));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeLitecoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeLitecoin));
    assertStringsEqual(symbol, "LTC");
    assertStringsEqual(txUrl, "https://blockchair.com/litecoin/transaction/123");
    assertStringsEqual(id, "litecoin");
    assertStringsEqual(name, "Litecoin");
}
