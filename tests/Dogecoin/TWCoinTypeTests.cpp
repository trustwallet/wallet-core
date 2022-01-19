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


TEST(TWDogecoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDoge));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDoge, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeDoge, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDoge));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDoge));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDoge), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDoge));
    ASSERT_EQ(0x16, TWCoinTypeP2shPrefix(TWCoinTypeDoge));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeDoge));
    assertStringsEqual(symbol, "DOGE");
    assertStringsEqual(txUrl, "https://blockchair.com/dogecoin/transaction/t123");
    assertStringsEqual(accUrl, "https://blockchair.com/dogecoin/address/a12");
    assertStringsEqual(id, "doge");
    assertStringsEqual(name, "Dogecoin");
}
