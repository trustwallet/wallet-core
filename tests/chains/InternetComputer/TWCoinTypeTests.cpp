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

TEST(TWInternetComputerCoinType, TWCoinType) {
    const auto coin = TWCoinTypeInternetComputer;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("9e32c54975adf84a1d98f19df41bbc34a752a899c32cc9c0000200b2c4308f85"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("529ea51c22e8d66e8302eabd9297b100fdb369109822248bb86939a671fbc55b"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "internet_computer");
    assertStringsEqual(name, "Internet Computer");
    assertStringsEqual(symbol, "ICP");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 8);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainInternetComputer);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://dashboard.internetcomputer.org/transaction/9e32c54975adf84a1d98f19df41bbc34a752a899c32cc9c0000200b2c4308f85");
    assertStringsEqual(accUrl, "https://dashboard.internetcomputer.org/account/529ea51c22e8d66e8302eabd9297b100fdb369109822248bb86939a671fbc55b");
}
