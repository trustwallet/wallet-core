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


TEST(TWAryacoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAryacoin));
    auto txId = TWStringCreateWithUTF8Bytes("fc381233e1d7eb42fe675f112fef8ff36b7f7796dc9a9a04e9dac5fa41499e0d");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAryacoin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAryacoin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAryacoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAryacoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAryacoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeAryacoin));
    ASSERT_EQ(50, TWCoinTypeP2shPrefix(TWCoinTypeAryacoin));
    ASSERT_EQ(0, TWCoinTypeStaticPrefix(TWCoinTypeAryacoin));
    assertStringsEqual(symbol, "AYA");
    assertStringsEqual(txUrl, "https://explorer2.aryacoin.io/tx/fc381233e1d7eb42fe675f112fef8ff36b7f7796dc9a9a04e9dac5fa41499e0d");
    assertStringsEqual(accUrl, "https://explorer2.aryacoin.io/address/AWWMxHsQUi58cSePL4EbBcodbUMZ1H1KNf");
    assertStringsEqual(id, "aryacoin");
    assertStringsEqual(name, "Aryacoin");
}
