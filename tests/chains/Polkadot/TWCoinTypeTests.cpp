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

TEST(TWPolkadotCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePolkadot));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x5aaaeec678ab1bd5a19d2c9bfd596037a32e3d1e831383c62f315459e85fa85d"));
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
    assertStringsEqual(txUrl, "https://assethub-polkadot.subscan.io/extrinsic/0x5aaaeec678ab1bd5a19d2c9bfd596037a32e3d1e831383c62f315459e85fa85d");
    assertStringsEqual(accUrl, "https://assethub-polkadot.subscan.io/account/13hJFqnkqQbmgnGQteGntjMjTdmTBRE8Z93JqxsrpgT7Yjd2");
    assertStringsEqual(id, "polkadot");
    assertStringsEqual(name, "Polkadot");
}
