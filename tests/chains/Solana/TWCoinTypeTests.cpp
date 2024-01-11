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


TEST(TWSolanaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSolana));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("5LmxrEKGchhMuYfw6Qut6CbsvE9pVfb8YvwZKvWssSesDVjHioBCmWKSJQh1WhvcM6CpemhpHNmEMA2a36rzwTa8"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSolana, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("Bxp8yhH9zNwxyE4UqxP7a7hgJ5xTZfxNNft7YJJ2VRjT"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeSolana, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSolana));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSolana));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSolana), 9);
    ASSERT_EQ(TWBlockchainSolana, TWCoinTypeBlockchain(TWCoinTypeSolana));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSolana));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSolana));
    assertStringsEqual(symbol, "SOL");
    assertStringsEqual(txUrl, "https://solscan.io/tx/5LmxrEKGchhMuYfw6Qut6CbsvE9pVfb8YvwZKvWssSesDVjHioBCmWKSJQh1WhvcM6CpemhpHNmEMA2a36rzwTa8");
    assertStringsEqual(accUrl, "https://solscan.io/account/Bxp8yhH9zNwxyE4UqxP7a7hgJ5xTZfxNNft7YJJ2VRjT");
    assertStringsEqual(id, "solana");
    assertStringsEqual(name, "Solana");
}
