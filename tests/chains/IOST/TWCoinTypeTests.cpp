// Copyright © 2017-2022 Trust Wallet.
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


TEST(TWIOSTCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIOST));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("7dKQzgTkPBNrZqrQ2Bqhqq132CHGPKANFDtzRsjHRCqx"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIOST, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("4av8w81EyzUgHonsVWqfs15WM4Vrpgox4BYYQWhNQDVu"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeIOST, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIOST));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIOST));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIOST), 2);
    ASSERT_EQ(TWBlockchainIOST, TWCoinTypeBlockchain(TWCoinTypeIOST));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeIOST));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeIOST));
    assertStringsEqual(symbol, "IOST");
    assertStringsEqual(txUrl, "https://explorer.iost.io/tx/7dKQzgTkPBNrZqrQ2Bqhqq132CHGPKANFDtzRsjHRCqx");
    assertStringsEqual(accUrl, "https://explorer.iost.io/account/4av8w81EyzUgHonsVWqfs15WM4Vrpgox4BYYQWhNQDVu");
    assertStringsEqual(id, "iost");
    assertStringsEqual(name, "IOST");
}
