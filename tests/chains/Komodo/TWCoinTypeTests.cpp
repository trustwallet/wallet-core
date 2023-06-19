// Copyright © 2017-2022 Trust Wallet.
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

namespace TW::Komodo::tests {

TEST(TWKomodoCoinType, TWCoinType) {
    const auto coin = TWCoinTypeKomodo;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("f53bd1a5c0f5dc4b60ba9a1882742ea96faa996e1b870795812a29604dd7829e"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("RWvfkt8UjbPWXgeZEcgYmKw2vA1bbAx5t2"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "komodo");
    assertStringsEqual(name, "Komodo");
    assertStringsEqual(symbol, "KMD");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 8);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainZcash);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x55);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(txUrl, "https://kmdexplorer.io//tx/f53bd1a5c0f5dc4b60ba9a1882742ea96faa996e1b870795812a29604dd7829e");
    assertStringsEqual(accUrl, "https://kmdexplorer.io//address/RWvfkt8UjbPWXgeZEcgYmKw2vA1bbAx5t2");
}

} // namespace TW::Komodo::tests