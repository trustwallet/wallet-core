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


TEST(TWAgoricCoinType, TWCoinType) {
    const auto coin = TWCoinTypeAgoric;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("576224B1A0F3D56BA23C5350C2A0E3CEA86F40005B828793E5ACBC2F4813152E"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("agoric1cqvwa8jr6pmt45jndanc8lqmdsxjkhw0yertc0"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "agoric");
    assertStringsEqual(name, "Agoric");
    assertStringsEqual(symbol, "BLD");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "agoric-3");
    assertStringsEqual(txUrl, "https://atomscan.com/agoric/transactions/576224B1A0F3D56BA23C5350C2A0E3CEA86F40005B828793E5ACBC2F4813152E");
    assertStringsEqual(accUrl, "https://atomscan.com/agoric/accounts/agoric1cqvwa8jr6pmt45jndanc8lqmdsxjkhw0yertc0");
}
