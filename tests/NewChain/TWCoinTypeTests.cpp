// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWNewChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNewChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNewChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x35552c16704d214347f29Fa77f77DA6d75d7C752"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNewChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNewChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNewChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNewChain), 18);
    ASSERT_EQ(TWBlockchainNewChain, TWCoinTypeBlockchain(TWCoinTypeNewChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNewChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNewChain));
    assertStringsEqual(symbol, "NEW");
    assertStringsEqual(txUrl, "https://explorer.newtonproject.org/tx/0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e");
    assertStringsEqual(accUrl, "https://explorer.newtonproject.org/address/0x35552c16704d214347f29Fa77f77DA6d75d7C752");
    assertStringsEqual(id, "newchain");
    assertStringsEqual(name, "NewChain");
}
