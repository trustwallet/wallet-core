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


TEST(TWTomoChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeTomoChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTomoChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTomoChain));
    assertStringsEqual(symbol, "TOMO");
    assertStringsEqual(txUrl, "https://scan.tomochain.com/txs/123");
    assertStringsEqual(id, "tomochain");
    assertStringsEqual(name, "TomoChain");
}
