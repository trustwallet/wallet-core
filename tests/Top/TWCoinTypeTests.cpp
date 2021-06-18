// Copyright © 2017-2020 Trust Wallet.
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

TEST(TWTopCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTop));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTop, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTop, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTop));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTop));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTop), 18);
    ASSERT_EQ(TWBlockchainTOP, TWCoinTypeBlockchain(TWCoinTypeTop));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTop));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTop));
    assertStringsEqual(symbol, "TOP");
    assertStringsEqual(txUrl, "https://topscan.io/transactions/t123");
    assertStringsEqual(accUrl, "https://topscan.io/accounts/a12");
    assertStringsEqual(id, "top");
    assertStringsEqual(name, "Top");
}
