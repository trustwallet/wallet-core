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


TEST(TWKavaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKava));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKava, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeKava, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKava));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKava));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKava), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeKava));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKava));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKava));
    assertStringsEqual(symbol, "KAVA");
    assertStringsEqual(txUrl, "https://kava.bigdipper.live/transactions/t123");
    assertStringsEqual(accUrl, "https://kava.bigdipper.live/account/a12");
    assertStringsEqual(id, "kava");
    assertStringsEqual(name, "Kava");
}