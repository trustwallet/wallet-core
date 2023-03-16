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


TEST(TWTomoChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x35a8d3ab06c94d5b7d27221b7c9a24ba3f1710dd0fcfd75c5d59b3a885fd709b"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x86cCbD9bfb371c355202086882bC644A7D0b024B"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTomoChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeTomoChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTomoChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTomoChain));
    assertStringsEqual(symbol, "TOMO");
    assertStringsEqual(txUrl, "https://tomoscan.io/tx/0x35a8d3ab06c94d5b7d27221b7c9a24ba3f1710dd0fcfd75c5d59b3a885fd709b");
    assertStringsEqual(accUrl, "https://tomoscan.io/address/0x86cCbD9bfb371c355202086882bC644A7D0b024B");
    assertStringsEqual(id, "tomochain");
    assertStringsEqual(name, "TomoChain");
}
