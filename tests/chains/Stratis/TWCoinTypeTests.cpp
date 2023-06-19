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


TEST(TWStratisCoinType, TWCoinType) {
    const auto coin = TWCoinTypeStratis;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("3923df87e83859dec8b87a414cbb1529113788c512a4d0c283e1394c274f0bfb"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("XWqnSWzQA5kDAS727UTYtXkdcbKc8mEvyN"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "stratis");
    assertStringsEqual(name, "Stratis");
    assertStringsEqual(symbol, "STRAX");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 8);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainBitcoin);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x8c);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://explorer.rutanio.com/strax/explorer/transaction/3923df87e83859dec8b87a414cbb1529113788c512a4d0c283e1394c274f0bfb");
    assertStringsEqual(accUrl, "https://explorer.rutanio.com/strax/explorer/address/XWqnSWzQA5kDAS727UTYtXkdcbKc8mEvyN");
}
