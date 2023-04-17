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

namespace TW::Cosmos::tests {
    TEST(TWUmeeCoinType, TWCoinType) {
        const auto coin = TWCoinTypeUmee;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("65B4B52C5F324F2287540847A114F645D89D544D99F793879FB3DBFF2CFEFC84"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("umee16934q0qf4akw8qruy5y8v748rvtxxjckgsecq4"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "umee");
        assertStringsEqual(name, "Umee");
        assertStringsEqual(symbol, "UMEE");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "umee-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/umee/txs/65B4B52C5F324F2287540847A114F645D89D544D99F793879FB3DBFF2CFEFC84");
        assertStringsEqual(accUrl, "https://www.mintscan.io/umee/account/umee16934q0qf4akw8qruy5y8v748rvtxxjckgsecq4");
    }
}
