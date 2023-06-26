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

namespace TW::Cosmos::tests {
    TEST(TWComdexCoinType, TWCoinType) {
        const auto coin = TWCoinTypeComdex;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("04C790D09A40EE958DBDD385B679B5EB60C10F9BC1389CC8F896DC9193A5ED6C"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("comdex1jz7av7cq45gh5hhrugtak7lkps2ga5v0u64nz6"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "comdex");
        assertStringsEqual(name, "Comdex");
        assertStringsEqual(symbol, "CMDX");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "comdex-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/comdex/txs/04C790D09A40EE958DBDD385B679B5EB60C10F9BC1389CC8F896DC9193A5ED6C");
        assertStringsEqual(accUrl, "https://www.mintscan.io/comdex/account/comdex1jz7av7cq45gh5hhrugtak7lkps2ga5v0u64nz6");
    }
}
