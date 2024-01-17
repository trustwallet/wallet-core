// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWBandChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBandChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("74AF38C2183B06EB6274DA4AAC0D2334E6E283643D436852F5E088AEA2CD0B17"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBandChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("band16gpgu994g2gdrzvwp9047le3pcq9wz6mcgtd4w"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBandChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBandChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBandChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBandChain), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeBandChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBandChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBandChain));
    assertStringsEqual(symbol, "BAND");
    assertStringsEqual(txUrl, "https://www.mintscan.io/band/tx/74AF38C2183B06EB6274DA4AAC0D2334E6E283643D436852F5E088AEA2CD0B17");
    assertStringsEqual(accUrl, "https://www.mintscan.io/band/account/band16gpgu994g2gdrzvwp9047le3pcq9wz6mcgtd4w");
    assertStringsEqual(id, "band");
    assertStringsEqual(name, "BandChain");
}
