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


TEST(TWVeChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeVeChain, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWBlockchainVechain, TWCoinTypeBlockchain(TWCoinTypeVeChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeVeChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeVeChain));
    assertStringsEqual(symbol, "VET");
    assertStringsEqual(txUrl, "https://insight.vecha.in/#/main/txs/t123");
    assertStringsEqual(accUrl, "https://insight.vecha.in/#/main/accounts/a12");
    assertStringsEqual(id, "vechain");
    assertStringsEqual(name, "VeChain");
}
