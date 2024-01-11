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


TEST(TWKuCoinCommunityChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKuCoinCommunityChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x2f0d79cd289a02f3181b68b9583a64c3809fe7387810b274275985c29d02c80d"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKuCoinCommunityChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4446fc4eb47f2f6586f9faab68b3498f86c07521"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeKuCoinCommunityChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKuCoinCommunityChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKuCoinCommunityChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKuCoinCommunityChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeKuCoinCommunityChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKuCoinCommunityChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKuCoinCommunityChain));
    assertStringsEqual(symbol, "KCS");
    assertStringsEqual(txUrl, "https://explorer.kcc.io/en/tx/0x2f0d79cd289a02f3181b68b9583a64c3809fe7387810b274275985c29d02c80d");
    assertStringsEqual(accUrl, "https://explorer.kcc.io/en/address/0x4446fc4eb47f2f6586f9faab68b3498f86c07521");
    assertStringsEqual(id, "kcc");
    assertStringsEqual(name, "KuCoin Community Chain");
}
