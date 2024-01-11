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
    TEST(TWMarsCoinType, TWCoinType) {
        const auto coin = TWCoinTypeMars;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("C12120760C71189A678739E0F1FD4EFAF2C29EA660B57A359AC728F89FAA7528"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("mars1nnjy6nct405pzfaqjm3dsyw0pf0kyw72vhw4pr"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "mars");
        assertStringsEqual(name, "Mars Hub");
        assertStringsEqual(symbol, "MARS");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "mars-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/mars-protocol/txs/C12120760C71189A678739E0F1FD4EFAF2C29EA660B57A359AC728F89FAA7528");
        assertStringsEqual(accUrl, "https://www.mintscan.io/mars-protocol/account/mars1nnjy6nct405pzfaqjm3dsyw0pf0kyw72vhw4pr");
    }
}
