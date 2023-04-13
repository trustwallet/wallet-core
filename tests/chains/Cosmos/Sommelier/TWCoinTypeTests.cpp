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


TEST(TWSommelierCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSommelier;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("E73A9E5E534777DDADF7F69A5CB41972894B862D1763FA4081FE913D8D3A5E80"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("somm10d5wmqvezwtj20u5hg3wuvwucce2nhsy0tzqgn"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "sommelier");
    assertStringsEqual(name, "Sommelier");
    assertStringsEqual(symbol, "SOMM");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "sommelier-3");
    assertStringsEqual(txUrl, "https://www.mintscan.io/sommelier/txs/E73A9E5E534777DDADF7F69A5CB41972894B862D1763FA4081FE913D8D3A5E80");
    assertStringsEqual(accUrl, "https://www.mintscan.io/sommelier/account/somm10d5wmqvezwtj20u5hg3wuvwucce2nhsy0tzqgn");
}
