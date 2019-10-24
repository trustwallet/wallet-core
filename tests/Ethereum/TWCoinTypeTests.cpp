// Copyright © 2017-2019 Trust Wallet.
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


TEST(TWEthereumCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereum));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereum, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereum));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereum));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereum));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeEthereum));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeEthereum));
    assertStringsEqual(symbol, "ETH");
    assertStringsEqual(txUrl, "https://etherscan.io/tx/123");
    assertStringsEqual(id, "ethereum");
    assertStringsEqual(name, "Ethereum");
}
