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


TEST(TWNEARCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEAR));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEAR, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNEAR, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEAR));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEAR));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEAR), 18);
    ASSERT_EQ(TWBlockchainNEAR, TWCoinTypeBlockchain(TWCoinTypeNEAR));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNEAR));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNEAR));
    assertStringsEqual(symbol, "NEAR");
    assertStringsEqual(txUrl, "https://explorer.nearprotocol.com/transactions/t123");
    assertStringsEqual(accUrl, "https://explorer.nearprotocol.com/accounts/a12");
    assertStringsEqual(id, "near");
    assertStringsEqual(name, "NEAR");
}
