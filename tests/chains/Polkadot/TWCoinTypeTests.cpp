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

TEST(TWPolkadotCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePolkadot));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb96f97d8ee508f420e606e1a6dcc74b88844713ddec2bd7cf4e3aa6b1d6beef4"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePolkadot, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("13hJFqnkqQbmgnGQteGntjMjTdmTBRE8Z93JqxsrpgT7Yjd2"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePolkadot, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePolkadot));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePolkadot));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePolkadot), 10);
    ASSERT_EQ(TWBlockchainPolkadot, TWCoinTypeBlockchain(TWCoinTypePolkadot));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePolkadot));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePolkadot));
    assertStringsEqual(symbol, "DOT");
    assertStringsEqual(txUrl, "https://polkadot.subscan.io/extrinsic/0xb96f97d8ee508f420e606e1a6dcc74b88844713ddec2bd7cf4e3aa6b1d6beef4");
    assertStringsEqual(accUrl, "https://polkadot.subscan.io/account/13hJFqnkqQbmgnGQteGntjMjTdmTBRE8Z93JqxsrpgT7Yjd2");
    assertStringsEqual(id, "polkadot");
    assertStringsEqual(name, "Polkadot");
}
