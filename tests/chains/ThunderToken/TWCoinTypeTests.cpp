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


TEST(TWThunderTokenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderCore));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderCore, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeThunderCore, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderCore));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderCore));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderCore), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeThunderCore));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeThunderCore));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeThunderCore));
    assertStringsEqual(symbol, "TT");
    assertStringsEqual(txUrl, "https://scan.thundercore.com/transactions/t123");
    assertStringsEqual(accUrl, "https://scan.thundercore.com/address/a12");
    assertStringsEqual(id, "thundertoken");
    assertStringsEqual(name, "ThunderCore");
}
