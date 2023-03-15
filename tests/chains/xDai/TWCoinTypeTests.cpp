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


TEST(TWxDaiCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDai));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x936798a1ef607c9e856d7861b15999c770c06f0887c4fc1f6acbf3bef09899c1"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXDai, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x12d61a95CF55e18D267C2F1AA67d8e42ae1368f8"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeXDai, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXDai));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXDai));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXDai), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeXDai));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXDai));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXDai));
    assertStringsEqual(symbol, "xDAI");
    assertStringsEqual(txUrl, "https://blockscout.com/xdai/mainnet/tx/0x936798a1ef607c9e856d7861b15999c770c06f0887c4fc1f6acbf3bef09899c1");
    assertStringsEqual(accUrl, "https://blockscout.com/xdai/mainnet/address/0x12d61a95CF55e18D267C2F1AA67d8e42ae1368f8");
    assertStringsEqual(id, "xdai");
    assertStringsEqual(name, "Gnosis Chain");
}
