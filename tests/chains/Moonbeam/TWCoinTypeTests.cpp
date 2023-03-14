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


TEST(TWMoonbeamCoinType, TWCoinType) {
    const auto coin = TWCoinTypeMoonbeam;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb22a146c933e6e51affbfa5f712a266b5f5e92ae453cd2f252bcc3c36ff035a6"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x201bb4f276C765dF7225e5A4153E17edD23a67eC"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "moonbeam");
    assertStringsEqual(name, "Moonbeam");
    assertStringsEqual(symbol, "GLMR");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1284");
    assertStringsEqual(txUrl, "https://moonscan.io/tx/0xb22a146c933e6e51affbfa5f712a266b5f5e92ae453cd2f252bcc3c36ff035a6");
    assertStringsEqual(accUrl, "https://moonscan.io/address/0x201bb4f276C765dF7225e5A4153E17edD23a67eC");
}
