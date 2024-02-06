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


TEST(TWOasisCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOasis));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("73dc977fdd8596d4a57e6feb891b21f5da3652d26815dc94f15f7420c298e29e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOasis, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("oasis1qrx376dmwuckmruzn9vq64n49clw72lywctvxdf4"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOasis, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOasis));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOasis));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOasis), 9);
    ASSERT_EQ(TWBlockchainOasisNetwork, TWCoinTypeBlockchain(TWCoinTypeOasis));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOasis));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOasis));
    assertStringsEqual(symbol, "ROSE");
    assertStringsEqual(txUrl, "https://oasisscan.com/transactions/73dc977fdd8596d4a57e6feb891b21f5da3652d26815dc94f15f7420c298e29e");
    assertStringsEqual(accUrl, "https://oasisscan.com/accounts/detail/oasis1qrx376dmwuckmruzn9vq64n49clw72lywctvxdf4");
    assertStringsEqual(id, "oasis");
    assertStringsEqual(name, "Oasis");
}
