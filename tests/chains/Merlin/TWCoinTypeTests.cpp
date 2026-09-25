// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWMerlinCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMerlin;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xca6f2891959b669237738dd0bc2c1051d966778c9de90b94165032ce58364212"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xf7e017b3f61bD3410A3b03D7DAD7699FD6780584"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "merlin");
    assertStringsEqual(name, "Merlin");
    assertStringsEqual(symbol, "BTC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://scan.merlinchain.io/tx/0xca6f2891959b669237738dd0bc2c1051d966778c9de90b94165032ce58364212");
    assertStringsEqual(accUrl, "https://scan.merlinchain.io/address/0xf7e017b3f61bD3410A3b03D7DAD7699FD6780584");
}
