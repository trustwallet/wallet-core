// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWHederaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeHedera));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x9edaf0f7d9c6629c31bbf0471fc07d696c73b566b93783f7e25d8d5d2b62fa4f"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeHedera, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x0000000000000000000000000000000000130ed3"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeHedera, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeHedera));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeHedera));
    const auto chainId = WRAPS(TWCoinTypeChainId(TWCoinTypeHedera));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeHedera), 10);
    ASSERT_EQ(TWBlockchainHedera, TWCoinTypeBlockchain(TWCoinTypeHedera));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeHedera));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeHedera));
    assertStringsEqual(symbol, "HBAR");
    assertStringsEqual(txUrl, "https://mainnet-public.mirrornode.hedera.com/api/v1/tx/0x9edaf0f7d9c6629c31bbf0471fc07d696c73b566b93783f7e25d8d5d2b62fa4f");
    assertStringsEqual(accUrl, "https://mainnet-public.mirrornode.hedera.com/api/v1/accounts/0x0000000000000000000000000000000000130ed3");
    assertStringsEqual(id, "ethereum");
    assertStringsEqual(name, "Hedera");
    assertStringsEqual(chainId, "1");
}
