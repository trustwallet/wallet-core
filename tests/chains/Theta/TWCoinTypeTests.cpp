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


TEST(TWThetaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTheta, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTheta, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTheta));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTheta));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTheta), 18);
    ASSERT_EQ(TWBlockchainTheta, TWCoinTypeBlockchain(TWCoinTypeTheta));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTheta));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTheta));
    assertStringsEqual(symbol, "THETA");
    assertStringsEqual(txUrl, "https://explorer.thetatoken.org/txs/t123");
    assertStringsEqual(accUrl, "https://explorer.thetatoken.org/account/a12");
    assertStringsEqual(id, "theta");
    assertStringsEqual(name, "Theta");
}
