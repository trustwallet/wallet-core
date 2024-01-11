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


TEST(TWGoChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGoChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGoChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeGoChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGoChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGoChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGoChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeGoChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeGoChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeGoChain));
    assertStringsEqual(symbol, "GO");
    assertStringsEqual(txUrl, "https://explorer.gochain.io/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.gochain.io/addr/a12");
    assertStringsEqual(id, "gochain");
    assertStringsEqual(name, "GoChain");
}
