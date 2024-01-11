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


TEST(TWWavesCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWaves));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWaves, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeWaves, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWaves));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWaves));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWaves), 8);
    ASSERT_EQ(TWBlockchainWaves, TWCoinTypeBlockchain(TWCoinTypeWaves));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeWaves));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeWaves));
    assertStringsEqual(symbol, "WAVES");
    assertStringsEqual(txUrl, "https://wavesexplorer.com/tx/t123");
    assertStringsEqual(accUrl, "https://wavesexplorer.com/address/a12");
    assertStringsEqual(id, "waves");
    assertStringsEqual(name, "Waves");
}
