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


TEST(TWRootstockCoinType, TWCoinType) {
    const auto coin = TWCoinTypeRootstock;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xeb8fa0488a655f8dc975153bffd066800bcaae5f21cf372356365b2a1d6d2288"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4e5dabc28e4a0f5e5b19fcb56b28c5a1989352c1"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "rootstock");
    assertStringsEqual(name, "Rootstock");
    assertStringsEqual(symbol, "RBTC");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "30");
    assertStringsEqual(txUrl, "https://explorer.rsk.co/tx/0xeb8fa0488a655f8dc975153bffd066800bcaae5f21cf372356365b2a1d6d2288");
    assertStringsEqual(accUrl, "https://explorer.rsk.co/address/0x4e5dabc28e4a0f5e5b19fcb56b28c5a1989352c1");
}
