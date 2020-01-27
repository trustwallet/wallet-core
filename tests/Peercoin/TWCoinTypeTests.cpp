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


TEST(TWPeercoinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePeercoin));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePeercoin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePeercoin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePeercoin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePeercoin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePeercoin), 6);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypePeercoin));
    ASSERT_EQ(0x75, TWCoinTypeP2shPrefix(TWCoinTypePeercoin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePeercoin));
    assertStringsEqual(symbol, "PPC");
    assertStringsEqual(txUrl, "https://blockbook.peercoin.net/tx/t123");
    assertStringsEqual(accUrl, "https://blockbook.peercoin.net/address/a12");
    assertStringsEqual(id, "peercoin");
    assertStringsEqual(name, "Peercoin");
}
