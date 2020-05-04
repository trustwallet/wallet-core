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


TEST(TWXinFinCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXinFin));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXinFin, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeXinFin, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXinFin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXinFin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXinFin), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeXinFin));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXinFin));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXinFin));
    assertStringsEqual(symbol, "XDC");
    assertStringsEqual(txUrl, "https://explorer.xinfin.network/tx/t123");
    assertStringsEqual(accUrl, "https://explorer.xinfin.network/addr/a12");
    assertStringsEqual(id, "xdc");
    assertStringsEqual(name, "XinFin");
}
