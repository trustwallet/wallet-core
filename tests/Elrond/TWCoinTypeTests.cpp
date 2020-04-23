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


TEST(TWElrondCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeElrond));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeElrond, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeElrond, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeElrond));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeElrond));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeElrond), 18);
    ASSERT_EQ(TWBlockchainElrondNetwork, TWCoinTypeBlockchain(TWCoinTypeElrond));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeElrond));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeElrond));
    assertStringsEqual(symbol, "ERD");
    assertStringsEqual(txUrl, "https://explorer.elrond.com/transactions/t123");
    assertStringsEqual(accUrl, "https://explorer.elrond.com/address/a12");
    assertStringsEqual(id, "elrond");
    assertStringsEqual(name, "Elrond");
}
