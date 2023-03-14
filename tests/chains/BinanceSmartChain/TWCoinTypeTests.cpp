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


TEST(TWBinanceSmartChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSmartChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSmartChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x35552c16704d214347f29Fa77f77DA6d75d7C752"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeSmartChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSmartChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSmartChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSmartChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeSmartChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSmartChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSmartChain));
    ASSERT_EQ(20000714, TWCoinTypeSmartChain);
    assertStringsEqual(symbol, "BNB");
    assertStringsEqual(txUrl, "https://bscscan.com/tx/0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e");
    assertStringsEqual(accUrl, "https://bscscan.com/address/0x35552c16704d214347f29Fa77f77DA6d75d7C752");
    assertStringsEqual(id, "smartchain");
    assertStringsEqual(name, "BNB Smart Chain");
}

TEST(TWBinanceSmartChainLegacyCoinType, TWCoinType) {
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSmartChainLegacy));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSmartChainLegacy));

    ASSERT_EQ(10000714, TWCoinTypeSmartChainLegacy);
    assertStringsEqual(id, "bsc");
    assertStringsEqual(name, "Smart Chain Legacy");
}
