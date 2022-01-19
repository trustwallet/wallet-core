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


TEST(TWAvalancheCoinType, TWCoinTypeCChain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAvalancheC));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x9243890b844219accefd8798271052f5a056453ec18984a56e81c92921330d54"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAvalancheC, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xa664325f36Ec33E66323fe2620AF3f2294b2Ef3A"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAvalancheC, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAvalancheC));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAvalancheC));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAvalancheC), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeAvalancheC));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAvalancheC));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAvalancheC));
    assertStringsEqual(symbol, "AVAX");
    assertStringsEqual(txUrl, "https://cchain.explorer.avax.network/tx/0x9243890b844219accefd8798271052f5a056453ec18984a56e81c92921330d54");
    assertStringsEqual(accUrl, "https://cchain.explorer.avax.network/address/0xa664325f36Ec33E66323fe2620AF3f2294b2Ef3A");
    assertStringsEqual(id, "avalanchec");
    assertStringsEqual(name, "Avalanche C-Chain");
}
