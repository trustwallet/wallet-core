// Copyright © 2017-2019 Trust Wallet.
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


TEST(TWXRPCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXRP));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXRP, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXRP));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXRP), 6);
    ASSERT_EQ(TWBlockchainRipple, TWCoinTypeBlockchain(TWCoinTypeXRP));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXRP));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXRP));
    assertStringsEqual(symbol, "XRP");
    assertStringsEqual(txUrl, "https://bithomp.com/explorer/123");
    assertStringsEqual(id, "ripple");
    assertStringsEqual(name, "XRP");
}
