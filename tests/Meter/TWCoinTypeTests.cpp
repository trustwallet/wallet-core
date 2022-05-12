// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWMeterCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMeter));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x31533707c3feb3b10f7deeea387ff8893f229253e65ca6b14d2400bf95b5d103"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMeter, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4F96F50eDB37a19216d87693E5dB241e31bD3735"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeMeter, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMeter));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMeter));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMeter), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeMeter));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeMeter));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeMeter));
    assertStringsEqual(symbol, "MTG");
    assertStringsEqual(txUrl, "https://scan.meter.io/tx/0x75f87730c0894eaf0c683415f0f058a0a1d7801f590dc495ca28e477fe769f56");
    assertStringsEqual(accUrl, "https://scan.meter.io/address/0x931bb8c7fb6cd099678fae36a5370577cee18ade");
    assertStringsEqual(id, "meter");
    assertStringsEqual(name, "Meter");
}
