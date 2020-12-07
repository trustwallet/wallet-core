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


TEST(TWNULSCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNULS));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNULS, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeNULS, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNULS));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNULS));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNULS), 8);
    ASSERT_EQ(TWBlockchainNULS, TWCoinTypeBlockchain(TWCoinTypeNULS));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeNULS));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeNULS));
    assertStringsEqual(symbol, "NULS");
    assertStringsEqual(txUrl, "https://nulscan.io/transaction/info?hash=t123");
    assertStringsEqual(accUrl, "https://nulscan.io/address/info?address=a12");
    assertStringsEqual(id, "nuls");
    assertStringsEqual(name, "NULS");
}
