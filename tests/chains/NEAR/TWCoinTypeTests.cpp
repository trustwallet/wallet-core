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


TEST(TWNEARCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEAR));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("FPQAMaVnvFHNwNBJWnTttXfdJhp5FvMGGDJEesB8gvbL"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEAR, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("test-trust.vlad.near"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNEAR, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEAR));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEAR));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEAR), 24);
    ASSERT_EQ(TWBlockchainNEAR, TWCoinTypeBlockchain(TWCoinTypeNEAR));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNEAR));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNEAR));
    assertStringsEqual(symbol, "NEAR");
    assertStringsEqual(txUrl, "https://explorer.near.org/transactions/FPQAMaVnvFHNwNBJWnTttXfdJhp5FvMGGDJEesB8gvbL");
    assertStringsEqual(accUrl, "https://explorer.near.org/accounts/test-trust.vlad.near");
    assertStringsEqual(id, "near");
    assertStringsEqual(name, "NEAR");
}
