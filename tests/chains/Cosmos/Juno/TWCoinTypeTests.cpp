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

namespace TW::Cosmos::tests {
    TEST(TWJunoCoinType, TWCoinType) {
        const auto coin = TWCoinTypeJuno;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("3DCE6AAF19657BCF11D44FD6BE124D57B44E04CA34851DE0ECCE619F70ECC46F"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "juno");
        assertStringsEqual(name, "Juno");
        assertStringsEqual(symbol, "JUNO");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "juno-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/juno/txs/3DCE6AAF19657BCF11D44FD6BE124D57B44E04CA34851DE0ECCE619F70ECC46F");
        assertStringsEqual(accUrl, "https://www.mintscan.io/juno/account/juno1mry47pkga5tdswtluy0m8teslpalkdq0gnn4mf");
    }
}
