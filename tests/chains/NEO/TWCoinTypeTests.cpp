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


TEST(TWNEOCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEO));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("e0ddf7c81c732df26180aca0c36d5868ad009fdbbe6e7a56ebafc14bba41cd53"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEO, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("AcxuqWhTureEQGeJgbmtSWNAtssjMLU7pb"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNEO, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEO));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEO));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEO), 8);
    ASSERT_EQ(TWBlockchainNEO, TWCoinTypeBlockchain(TWCoinTypeNEO));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNEO));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNEO));
    assertStringsEqual(symbol, "NEO");
    assertStringsEqual(txUrl, "https://neoscan.io/transaction/e0ddf7c81c732df26180aca0c36d5868ad009fdbbe6e7a56ebafc14bba41cd53");
    assertStringsEqual(accUrl, "https://neoscan.io/address/AcxuqWhTureEQGeJgbmtSWNAtssjMLU7pb");
    assertStringsEqual(id, "neo");
    assertStringsEqual(name, "NEO");
}
