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


TEST(TWAxelarCoinType, TWCoinType) {
    const auto coin = TWCoinTypeAxelar;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("838F31D023B273E6A8282085202A4CCEDE1693D2503ACCD557B37C9DAB33A79C"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "axelar");
    assertStringsEqual(name, "Axelar");
    assertStringsEqual(symbol, "AXL");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "axelar-dojo-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/axelar/txs/838F31D023B273E6A8282085202A4CCEDE1693D2503ACCD557B37C9DAB33A79C");
    assertStringsEqual(accUrl, "https://www.mintscan.io/axelar/account/axelar1mry47pkga5tdswtluy0m8teslpalkdq060xxh5");
}
