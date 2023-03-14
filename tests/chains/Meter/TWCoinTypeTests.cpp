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


TEST(TWMeterCoinType, TWCoinType) {
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMeter));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x8ea268d5dbb40217c763b800a75fc063cf28b56f40f2bc69dc043f5c4bbdc144"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMeter, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xe5a273954d24eddf9ae9ea4cef2347d584cfa3dd"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMeter, accId.get()));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMeter));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMeter));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMeter), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeMeter));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeMeter));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMeter));
    assertStringsEqual(symbol, "MTR");
    assertStringsEqual(txUrl, "https://scan.meter.io/tx/0x8ea268d5dbb40217c763b800a75fc063cf28b56f40f2bc69dc043f5c4bbdc144");
    assertStringsEqual(accUrl, "https://scan.meter.io/address/0xe5a273954d24eddf9ae9ea4cef2347d584cfa3dd");
    assertStringsEqual(id, "meter");
    assertStringsEqual(name, "Meter");
}
