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


TEST(TWStrideCoinType, TWCoinType) {
    const auto coin = TWCoinTypeStride;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("FB67755B3A00D4BCC11F607867B9C767CF24BCB749C718579D1EC794226087C8"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("stride1c44mngg9pjjeqrr07sle7ntuggrajnt4lsf9jl"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "stride");
    assertStringsEqual(name, "Stride");
    assertStringsEqual(symbol, "STRD");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "stride-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/stride/txs/FB67755B3A00D4BCC11F607867B9C767CF24BCB749C718579D1EC794226087C8");
    assertStringsEqual(accUrl, "https://www.mintscan.io/stride/account/stride1c44mngg9pjjeqrr07sle7ntuggrajnt4lsf9jl");
}
