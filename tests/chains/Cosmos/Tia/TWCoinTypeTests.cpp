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

TEST(TWTiaCoinType, TWCoinType) {
    const auto coin = TWCoinTypeTia;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("FF370C65D8D67B8236F9D3A8D2B1256337C60C1965092CADD1FA970288FCE99B"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("celestia1tt4tv4jrs4twdtzwywxd8u65duxgk8y73wvfu2"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));
    assertStringsEqual(id, "tia");
    assertStringsEqual(name, "Celestia");
    assertStringsEqual(symbol, "TIA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "celestia");
    assertStringsEqual(txUrl, "https://www.mintscan.io/celestia/txs/FF370C65D8D67B8236F9D3A8D2B1256337C60C1965092CADD1FA970288FCE99B");
    assertStringsEqual(accUrl, "https://www.mintscan.io/celestia/account/celestia1tt4tv4jrs4twdtzwywxd8u65duxgk8y73wvfu2");
}

} // namespace TW::Cosmos::tests
