// Copyright © 2017-2020 Trust Wallet.
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


TEST(TWPermissionCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePermission));
    auto txId = TWStringCreateWithUTF8Bytes("0x42f7a92223c2772686ed688ac26a16ab5ba3b4a187463d4f56f11bf3ea9967e4");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePermission, txId));
    auto accId = TWStringCreateWithUTF8Bytes("0x4faF226eA0437A14ae882Fc05Df2439029312E3E");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePermission, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePermission));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePermission));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePermission), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePermission));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePermission));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePermission));
    assertStringsEqual(symbol, "ASK");
    assertStringsEqual(txUrl, "https://explorer.permission.io//transactions/0x42f7a92223c2772686ed688ac26a16ab5ba3b4a187463d4f56f11bf3ea9967e4");
    assertStringsEqual(accUrl, "https://explorer.permission.io//wallets/0x4faF226eA0437A14ae882Fc05Df2439029312E3E");
    assertStringsEqual(id, "permission");
    assertStringsEqual(name, "Permission");
}
