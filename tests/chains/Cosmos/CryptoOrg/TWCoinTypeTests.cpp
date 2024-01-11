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


TEST(TWCryptoorgCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCryptoOrg));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("D87D2EB46B21466886EE149C1DEA3AE6C2E019C7D8C24FA1533A95439DDCE1E2"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCryptoOrg, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("cro10wrflcdc4pys9vvgqm98yg7yv5ltj7d3xehent"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCryptoOrg, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCryptoOrg));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCryptoOrg));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCryptoOrg), 8);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCryptoOrg));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCryptoOrg));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCryptoOrg));
    assertStringsEqual(symbol, "CRO");
    assertStringsEqual(txUrl, "https://crypto.org/explorer/tx/D87D2EB46B21466886EE149C1DEA3AE6C2E019C7D8C24FA1533A95439DDCE1E2");
    assertStringsEqual(accUrl, "https://crypto.org/explorer/account/cro10wrflcdc4pys9vvgqm98yg7yv5ltj7d3xehent");
    assertStringsEqual(id, "cryptoorg");
    assertStringsEqual(name, "Crypto.org");
}
