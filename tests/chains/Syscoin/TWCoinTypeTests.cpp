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


TEST(TWSyscoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSyscoin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("19e043f76f6ffc960f5fe93ecec37bc37a58ae7525d7e9cd6ed40f71f0da60eb"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSyscoin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("sys1qh3gvhnzq2ch7w8g04x8zksr2mz7r90x7ksmu40"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeSyscoin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSyscoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSyscoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSyscoin), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeSyscoin));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeSyscoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSyscoin));
    assertStringsEqual(symbol, "SYS");
    assertStringsEqual(txUrl, "https://sys1.bcfn.ca/tx/19e043f76f6ffc960f5fe93ecec37bc37a58ae7525d7e9cd6ed40f71f0da60eb");
    assertStringsEqual(accUrl, "https://sys1.bcfn.ca/address/sys1qh3gvhnzq2ch7w8g04x8zksr2mz7r90x7ksmu40");
    assertStringsEqual(id, "syscoin");
    assertStringsEqual(name, "Syscoin");
}
