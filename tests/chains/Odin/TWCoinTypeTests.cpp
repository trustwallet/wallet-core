// Copyright © 2017-2021 Trust Wallet.
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

TEST(TWOdinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOdin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes(
        "622625863733be4c5c8c3709ce114bc6526ab6a9a708fc054ffbab3a0a34ca07"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOdin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("odin14pcd29hqlxjq29hqmeu99xfpxf70hj8w53rr2m"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOdin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOdin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOdin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOdin), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeOdin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOdin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOdin));
    assertStringsEqual(symbol, "ODIN");
    assertStringsEqual(txUrl, "https://scan.odinprotocol.io/transactions/"
                              "622625863733be4c5c8c3709ce114bc6526ab6a9a708fc054ffbab3a0a34ca07");
    assertStringsEqual(
        accUrl,
        "https://scan.odinprotocol.io/accounts/odin14pcd29hqlxjq29hqmeu99xfpxf70hj8w53rr2m");
    assertStringsEqual(id, "odin");
    assertStringsEqual(name, "Odin");
}
