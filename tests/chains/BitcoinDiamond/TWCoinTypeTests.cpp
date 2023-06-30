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


TEST(TWBitcoinDiamondCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinDiamond));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("ec564fe8993ba77f3f5c8b7f6ebb4cbc08e564a54612d6f4584cd1017cf723d4"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinDiamond, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("1HNTyntGXNhy4WxNzWfffPqp7LHb8bGJ9R"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBitcoinDiamond, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinDiamond));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinDiamond));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinDiamond), 7);
    ASSERT_EQ(TWBlockchainBitcoinDiamond, TWCoinTypeBlockchain(TWCoinTypeBitcoinDiamond));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinDiamond));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBitcoinDiamond));
    assertStringsEqual(symbol, "BCD");
    assertStringsEqual(txUrl, "http://explorer.btcd.io/#/tx?tx=ec564fe8993ba77f3f5c8b7f6ebb4cbc08e564a54612d6f4584cd1017cf723d4");
    assertStringsEqual(accUrl, "http://explorer.btcd.io/#/address?address=1HNTyntGXNhy4WxNzWfffPqp7LHb8bGJ9R");
    assertStringsEqual(id, "bitcoindiamond");
    assertStringsEqual(name, "Bitcoin Diamond");
}
