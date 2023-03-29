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


TEST(TWECashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeECash));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("6bc767e69cfacffd954c9e5acd178d3140bf00d094b92c6f6052b517500c553b"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeECash, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("ecash:pqnqv9lt7e5vjyp0w88zf2af0l92l8rxdg2jj94l5j"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeECash, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeECash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeECash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeECash), 2);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeECash));
    ASSERT_EQ(0x5, TWCoinTypeP2shPrefix(TWCoinTypeECash));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeECash));
    assertStringsEqual(symbol, "XEC");
    assertStringsEqual(txUrl, "https://explorer.bitcoinabc.org/tx/6bc767e69cfacffd954c9e5acd178d3140bf00d094b92c6f6052b517500c553b");
    assertStringsEqual(accUrl, "https://explorer.bitcoinabc.org/address/ecash:pqnqv9lt7e5vjyp0w88zf2af0l92l8rxdg2jj94l5j");
    assertStringsEqual(id, "ecash");
    assertStringsEqual(name, "eCash");
}
