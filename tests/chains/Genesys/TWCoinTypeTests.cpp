// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWGenesysType, TWCoinType) {
    const auto coin = TWCoinTypeGenesys;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x717b7310d621ef669d02d516ab111c0baac30e164f554a41856d6c5d98fe9a2f"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xC95B4bd9670A82035E4707E0E1BdE9Fc9238BA79"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "genesys");
    assertStringsEqual(name, "Genesys");
    assertStringsEqual(symbol, "GSYS");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2pkhPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0);
    assertStringsEqual(txUrl, "https://bbscan.io/tx/0x717b7310d621ef669d02d516ab111c0baac30e164f554a41856d6c5d98fe9a2f");
    assertStringsEqual(accUrl, "https://bbscan.io/address/0xC95B4bd9670A82035E4707E0E1BdE9Fc9238BA79");
}
