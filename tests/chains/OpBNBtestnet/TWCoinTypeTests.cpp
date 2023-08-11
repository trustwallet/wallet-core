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


TEST(TWOpBNBtestnetCoinType, TWCoinType) {
    const auto coin = TWCoinTypeOpBNB;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x788ea8fb4a82dae957f1d3b18af3cd0bbde55a276e66bd17af8c869f24c03a0f"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x4eaf936c172b5e5511959167e8ab4f7031113ca3"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "opbnb");
    assertStringsEqual(name, "OpBNB");
    assertStringsEqual(symbol, "BNB");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "204");
    assertStringsEqual(txUrl, "https://opbnbscan.com/tx/0x788ea8fb4a82dae957f1d3b18af3cd0bbde55a276e66bd17af8c869f24c03a0f");
    assertStringsEqual(accUrl, "https://opbnbscan.com/address/0x4eaf936c172b5e5511959167e8ab4f7031113ca3");
}
