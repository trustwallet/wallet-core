// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWKCCCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWKCCCoinType));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x2f0d79cd289a02f3181b68b9583a64c3809fe7387810b274275985c29d02c80d"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWKCCCoinType, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4446fc4eb47f2f6586f9faab68b3498f86c07521"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWKCCCoinType, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWKCCCoinType));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWKCCCoinType));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWKCCCoinType), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWKCCCoinType));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWKCCCoinType));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWKCCCoinType));
    assertStringsEqual(symbol, "KCS");
    assertStringsEqual(txUrl, "https://explorer.kcc.io/en/tx/0x2f0d79cd289a02f3181b68b9583a64c3809fe7387810b274275985c29d02c80d");
    assertStringsEqual(accUrl, "https://explorer.kcc.io/en/address/0x4446fc4eb47f2f6586f9faab68b3498f86c07521");
    assertStringsEqual(id, "kcc");
    assertStringsEqual(name, "KCC");
}
