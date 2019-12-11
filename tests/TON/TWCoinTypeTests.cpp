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


TEST(TWTONCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTON));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTON, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTON, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTON));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTON));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTON), 9);
    ASSERT_EQ(TWBlockchainTON, TWCoinTypeBlockchain(TWCoinTypeTON));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTON));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTON));
    assertStringsEqual(symbol, "GRAM");
    assertStringsEqual(txUrl, "https://test.ton.org/testnet/transaction?hash=t123");
    assertStringsEqual(accUrl, "https://test.ton.org/testnet/account?account=a12");
    assertStringsEqual(id, "ton");
    assertStringsEqual(name, "TON");
}
