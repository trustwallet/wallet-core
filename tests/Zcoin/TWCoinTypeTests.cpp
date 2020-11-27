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


TEST(TWZcoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    auto txId = TWStringCreateWithUTF8Bytes("09a60d58b3d17519a42a8eca60750c33b710ca8f3ca71994192e05c248a2a111");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a8ULhhDgfdSiXJhSZVdhb8EuDc6R3ogsaM");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZcoin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcoin));
    ASSERT_EQ(0x7, TWCoinTypeP2shPrefix(TWCoinTypeZcoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeZcoin));
    assertStringsEqual(symbol, "FIRO");
    assertStringsEqual(txUrl, "https://explorer.firo.org/tx/09a60d58b3d17519a42a8eca60750c33b710ca8f3ca71994192e05c248a2a111");
    assertStringsEqual(accUrl, "https://explorer.firo.org/address/a8ULhhDgfdSiXJhSZVdhb8EuDc6R3ogsaM");
    assertStringsEqual(id, "zcoin");
    assertStringsEqual(name, "Firo");
}
