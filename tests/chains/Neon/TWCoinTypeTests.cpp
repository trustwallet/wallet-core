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


TEST(TWNeonCoinType, TWCoinType) {
    const auto coin = TWCoinTypeNeon;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x77d86af2c6f02f14ef13ca52bf54864d92fcc4b32d8e884e225061c006738ed6"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xfa4a8650e7bebb918859c280a86f9661bed29877"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "neon");
    assertStringsEqual(name, "Neon");
    assertStringsEqual(symbol, "NEON");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "245022934");
    assertStringsEqual(txUrl, "https://neonscan.org/tx/0x77d86af2c6f02f14ef13ca52bf54864d92fcc4b32d8e884e225061c006738ed6");
    assertStringsEqual(accUrl, "https://neonscan.org/address/0xfa4a8650e7bebb918859c280a86f9661bed29877");
}
