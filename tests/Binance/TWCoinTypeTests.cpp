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


TEST(TWBinanceCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinance));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinance));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinance), 8);
    ASSERT_EQ(TWBlockchainBinance, TWCoinTypeBlockchain(TWCoinTypeBinance));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBinance));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBinance));
    assertStringsEqual(symbol, "BNB");
    assertStringsEqual(txUrl, "https://explorer.binance.org/tx/123");
    assertStringsEqual(id, "binance");
    assertStringsEqual(name, "BNB");
}
