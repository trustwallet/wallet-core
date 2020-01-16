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


TEST(TWPolkadotCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePolkadot));

    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePolkadot, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePolkadot, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePolkadot));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePolkadot));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePolkadot), 15);
    ASSERT_EQ(TWBlockchainPolkadot, TWCoinTypeBlockchain(TWCoinTypePolkadot));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePolkadot));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePolkadot));
    assertStringsEqual(symbol, "DOT");
    assertStringsEqual(txUrl, "https://polkadot.subscan.io/extrinsic/t123");
    assertStringsEqual(accUrl, "https://polkadot.subscan.io/account/a12");
    assertStringsEqual(id, "polkadot");
    assertStringsEqual(name, "Polkadot");
}
