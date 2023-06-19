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

TEST(TWOptimismCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOptimism));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x6fd99288be9bf71eb002bb31da10a4fb0fbbb3c45ae73693b212f49c9db7df8f"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOptimism, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x1f932361e31d206b4f6b2478123a9d0f8c761031"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOptimism, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOptimism));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOptimism));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOptimism), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeOptimism));

    assertStringsEqual(symbol, "ETH");
    assertStringsEqual(txUrl, "https://optimistic.etherscan.io/tx/0x6fd99288be9bf71eb002bb31da10a4fb0fbbb3c45ae73693b212f49c9db7df8f");
    assertStringsEqual(accUrl, "https://optimistic.etherscan.io/address/0x1f932361e31d206b4f6b2478123a9d0f8c761031");
    assertStringsEqual(id, "optimism");
    assertStringsEqual(name, "Optimism Ethereum");
}
