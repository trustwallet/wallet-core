// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//
#include "../common/TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWNeblCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNebl));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("53713d5208e2a1bc68ef8bd5c851f113cf5e1675d22ecf3406f38819ab9187b3"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNebl, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNebl, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNebl));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNebl));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNebl), 8);
    ASSERT_EQ(TWBlockchainNebl, TWCoinTypeBlockchain(TWCoinTypeNebl));
    ASSERT_EQ(0x70, TWCoinTypeP2shPrefix(TWCoinTypeNebl));
    ASSERT_EQ(0x35, TWCoinTypeP2pkhPrefix(TWCoinTypeNebl));
    assertStringsEqual(symbol, "NEBL");
    assertStringsEqual(txUrl, "https://explorer.nebl.io/tx/53713d5208e2a1bc68ef8bd5c851f113cf5e1675d22ecf3406f38819ab9187b3");
    assertStringsEqual(accUrl, "https://explorer.nebl.io/address/NboLGGKWtK5eXzaah5GVpXju9jCcoMi4cc");
    assertStringsEqual(id, "Nebl");
    assertStringsEqual(name, "Nebl");
}
