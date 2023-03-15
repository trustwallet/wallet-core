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


TEST(TWBitcoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0607f62530b68cfcc91c57a1702841dd399a899d0eecda8e31ecca3f52f01df2"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("17A16QmavnUfCW11DAApiJxp7ARnxN5pGX"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBitcoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoin));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeBitcoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBitcoin));
    assertStringsEqual(symbol, "BTC");
    assertStringsEqual(txUrl, "https://blockchair.com/bitcoin/transaction/0607f62530b68cfcc91c57a1702841dd399a899d0eecda8e31ecca3f52f01df2");
    assertStringsEqual(accUrl, "https://blockchair.com/bitcoin/address/17A16QmavnUfCW11DAApiJxp7ARnxN5pGX");
    assertStringsEqual(id, "bitcoin");
    assertStringsEqual(name, "Bitcoin");
}
