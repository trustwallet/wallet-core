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


TEST(TWPlatonCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePlaton));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x9edaf0f7d9c6629c31bbf0471fc07d696c73b566b93783f7e25d8d5d2b62fa4f"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePlaton, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x5bb497e8d9fe26e92dd1be01e32076c8e024d167"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePlaton, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePlaton));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePlaton));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePlaton), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePlaton));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePlaton));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePlaton));
    assertStringsEqual(symbol, "LAT");
    assertStringsEqual(txUrl, "https://scan.platon.network//tx/0x9edaf0f7d9c6629c31bbf0471fc07d696c73b566b93783f7e25d8d5d2b62fa4f");
    assertStringsEqual(accUrl, "https://scan.platon.network//address/0x5bb497e8d9fe26e92dd1be01e32076c8e024d167");
    assertStringsEqual(id, "platon");
    assertStringsEqual(name, "Platon");
}
