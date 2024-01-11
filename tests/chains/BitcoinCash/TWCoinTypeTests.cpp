// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWBitcoinCashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinCash));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinCash, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBitcoinCash, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinCash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinCash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoinCash));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinCash));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBitcoinCash));
    assertStringsEqual(symbol, "BCH");
    assertStringsEqual(txUrl, "https://blockchair.com/bitcoin-cash/transaction/t123");
    assertStringsEqual(accUrl, "https://blockchair.com/bitcoin-cash/address/a12");
    assertStringsEqual(id, "bitcoincash");
    assertStringsEqual(name, "Bitcoin Cash");
}
