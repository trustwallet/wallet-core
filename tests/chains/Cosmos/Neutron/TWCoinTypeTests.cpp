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


TEST(TWNeutronCoinType, TWCoinType) {
    const auto coin = TWCoinTypeNeutron;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("E18BA087009A05EB6A15A22FE30BA99379B909F74A74120E6F92B9882C45F0D7"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("neutron1pm4af8pcurxssdxztqw9rexx5f8zfq7uzqfmy8"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "neutron");
    assertStringsEqual(name, "Neutron");
    assertStringsEqual(symbol, "NTRN");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "neutron-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/neutron/txs/E18BA087009A05EB6A15A22FE30BA99379B909F74A74120E6F92B9882C45F0D7");
    assertStringsEqual(accUrl, "https://www.mintscan.io/neutron/account/neutron1pm4af8pcurxssdxztqw9rexx5f8zfq7uzqfmy8");
}
