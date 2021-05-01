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


TEST(TWMeterMetaStableGasTokenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWMeterMetaStableGasTokenCoinType));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x33d61f9489655747f817fd2a227b6ef67eef4126835780fc446ab82849382fbb"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWMeterMetaStableGasTokenCoinType, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xe9061c2517bba8a7e2d2c20053cd8323b577efe7"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWMeterMetaStableGasTokenCoinType, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWMeterMetaStableGasTokenCoinType));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWMeterMetaStableGasTokenCoinType));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWMeterMetaStableGasTokenCoinType), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWMeterMetaStableGasTokenCoinType));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWMeterMetaStableGasTokenCoinType));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWMeterMetaStableGasTokenCoinType));
    assertStringsEqual(symbol, "MTR");
    assertStringsEqual(txUrl, "https://scan.meter.io/tx/0x33d61f9489655747f817fd2a227b6ef67eef4126835780fc446ab82849382fbb");
    assertStringsEqual(accUrl, "https://scan.meter.io/address/0xe9061c2517bba8a7e2d2c20053cd8323b577efe7");
    assertStringsEqual(id, "Meter");
    assertStringsEqual(name, "Meter MetaStable Gas Token");
}
