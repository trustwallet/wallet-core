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


TEST(TWAvalancheCoinType, TWCoinTypeCChain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAvalancheCChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x9243890b844219accefd8798271052f5a056453ec18984a56e81c92921330d54"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAvalancheCChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xa664325f36Ec33E66323fe2620AF3f2294b2Ef3A"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAvalancheCChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAvalancheCChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAvalancheCChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAvalancheCChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeAvalancheCChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAvalancheCChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAvalancheCChain));
    assertStringsEqual(symbol, "AVAX");
    assertStringsEqual(txUrl, "https://snowtrace.io/tx/0x9243890b844219accefd8798271052f5a056453ec18984a56e81c92921330d54");
    assertStringsEqual(accUrl, "https://snowtrace.io/address/0xa664325f36Ec33E66323fe2620AF3f2294b2Ef3A");
    assertStringsEqual(id, "avalanchec");
    assertStringsEqual(name, "Avalanche C-Chain");
}
