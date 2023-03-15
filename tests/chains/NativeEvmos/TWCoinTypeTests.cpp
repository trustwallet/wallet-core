// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::NativeEvmos::tests {

TEST(TWEvmosCoinType, TWCoinTypeNativeEvmos) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNativeEvmos));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNativeEvmos, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNativeEvmos, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNativeEvmos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNativeEvmos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNativeEvmos), 18);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeNativeEvmos));

    assertStringsEqual(symbol, "EVMOS");
    assertStringsEqual(txUrl, "https://mintscan.io/evmos/txs/A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811");
    assertStringsEqual(accUrl, "https://mintscan.io/evmos/account/evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34");
    assertStringsEqual(id, "nativeevmos");
    assertStringsEqual(name, "Native Evmos");
}

} // namespace TW::NativeEvmos::tests
