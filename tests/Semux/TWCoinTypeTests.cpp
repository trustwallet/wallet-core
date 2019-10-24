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


TEST(TWSemuxCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSemux));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSemux, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSemux));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSemux));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSemux), 9);
    ASSERT_EQ(TWBlockchainSemux, TWCoinTypeBlockchain(TWCoinTypeSemux));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSemux));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSemux));
    assertStringsEqual(symbol, "SEM");
    assertStringsEqual(txUrl, "https://semux.info/explorer/transaction/123");
    assertStringsEqual(id, "semux");
    assertStringsEqual(name, "Semux");
}
