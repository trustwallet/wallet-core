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


TEST(TWZelcashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZelcash));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZelcash, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZelcash, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZelcash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZelcash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZelcash), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZelcash));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZelcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZelcash));
    assertStringsEqual(symbol, "ZEL");
    assertStringsEqual(txUrl, "https://explorer.zel.cash/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.zel.cash/address/a12");
    assertStringsEqual(id, "zelcash");
    assertStringsEqual(name, "Zelcash");
}
