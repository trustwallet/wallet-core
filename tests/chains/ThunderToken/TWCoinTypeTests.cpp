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


TEST(TWThunderTokenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeThunderToken, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeThunderToken));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeThunderToken));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeThunderToken));
    assertStringsEqual(symbol, "TT");
    assertStringsEqual(txUrl, "https://scan.thundercore.com/transactions/t123");
    assertStringsEqual(accUrl, "https://scan.thundercore.com/address/a12");
    assertStringsEqual(id, "thundertoken");
    assertStringsEqual(name, "Thunder Token");
}
