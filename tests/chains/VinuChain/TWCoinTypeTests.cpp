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


TEST(TWVinuChainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeVinuChain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb0a741d882291951de1fac72e90b9baf886ddb0c9c87658a0c206490dfaa5202"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9474feb9917b87da6f0d830ba66ee0035835c0d3"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "vinuchain");
    assertStringsEqual(name, "VinuChain");
    assertStringsEqual(symbol, "VC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "207");
    assertStringsEqual(txUrl, "https://vinuexplorer.org/tx/0xb0a741d882291951de1fac72e90b9baf886ddb0c9c87658a0c206490dfaa5202");
    assertStringsEqual(accUrl, "https://vinuexplorer.org/address/0x9474feb9917b87da6f0d830ba66ee0035835c0d3");
}
