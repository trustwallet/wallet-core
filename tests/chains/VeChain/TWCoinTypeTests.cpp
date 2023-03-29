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


TEST(TWVeChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xa424053be0063555aee73a595ca69968c2e4d90d36f280753e503b92b11a655d"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x8a0a035a33173601bfbec8b6ae7c4a6557a55103"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeVeChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWBlockchainVechain, TWCoinTypeBlockchain(TWCoinTypeVeChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeVeChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeVeChain));
    assertStringsEqual(symbol, "VET");
    assertStringsEqual(txUrl, "https://explore.vechain.org/transactions/0xa424053be0063555aee73a595ca69968c2e4d90d36f280753e503b92b11a655d");
    assertStringsEqual(accUrl, "https://explore.vechain.org/accounts/0x8a0a035a33173601bfbec8b6ae7c4a6557a55103");
    assertStringsEqual(id, "vechain");
    assertStringsEqual(name, "VeChain");
}
