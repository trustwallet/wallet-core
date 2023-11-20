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


TEST(TWUltraProChainCoinType, TWCoinType) {
    const auto coin = TWCoinTypeUltraProChain;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x4e99f3ab0b9cd33a46e284a47acc0ebc7259b98b87c2aef4f2427270fa3ff8ad"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xF59Bf1c3E85d0b66D18B51a52cE3eE71872aC4ff"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "ultra_pro_chain");
    assertStringsEqual(name, "Ultra Pro Chain");
    assertStringsEqual(symbol, "UPRO");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "473861");
    assertStringsEqual(txUrl, "https://ultraproscan.io/tx/0x4e99f3ab0b9cd33a46e284a47acc0ebc7259b98b87c2aef4f2427270fa3ff8ad");
    assertStringsEqual(accUrl, "https://ultraproscan.io/address/0xF59Bf1c3E85d0b66D18B51a52cE3eE71872aC4ff");
}
