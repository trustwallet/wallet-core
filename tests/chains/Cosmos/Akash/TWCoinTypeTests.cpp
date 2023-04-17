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
    TEST(TWAkashCoinType, TWCoinType) {
        const auto coin = TWCoinTypeAkash;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("C0083856344425908D5333D4325E3E0DE9D697BA568C6D99C34303819F615D25"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("akash1f4nskxfw8ufhwnajh7xwt0wmdtxm02vwta6krg"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "akash");
        assertStringsEqual(name, "Akash");
        assertStringsEqual(symbol, "AKT");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "akashnet-2");
        assertStringsEqual(txUrl, "https://www.mintscan.io/akash/txs/C0083856344425908D5333D4325E3E0DE9D697BA568C6D99C34303819F615D25");
        assertStringsEqual(accUrl, "https://www.mintscan.io/akash/account/akash1f4nskxfw8ufhwnajh7xwt0wmdtxm02vwta6krg");
    }
}
