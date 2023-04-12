// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::NativeCanto::tests {

TEST(TWCantoCoinType, TWCoinTypeNativeCanto) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNativeCanto));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNativeCanto, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("canto17xpfvakm2amg962yls6f84z3kell8c5ljcjw34"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNativeCanto, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNativeCanto));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNativeCanto));
    const auto chainId = WRAPS(TWCoinTypeChainId(TWCoinTypeNativeCanto));


    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNativeCanto), 18);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeNativeCanto));

    assertStringsEqual(symbol, "CANTO");
    assertStringsEqual(txUrl, "https://mintscan.io/canto/txs/A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811");
    assertStringsEqual(accUrl, "https://mintscan.io/canto/account/canto17xpfvakm2amg962yls6f84z3kell8c5ljcjw34");
    assertStringsEqual(id, "nativecanto");
    assertStringsEqual(name, "NativeCanto");
    assertStringsEqual(chainId, "canto_7700-1");
}

} // namespace TW::NativeCanto::tests
