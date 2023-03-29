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


TEST(TWDigiByteCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDigiByte));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDigiByte, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeDigiByte, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDigiByte));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDigiByte));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDigiByte), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDigiByte));
    ASSERT_EQ(0x3f, TWCoinTypeP2shPrefix(TWCoinTypeDigiByte));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeDigiByte));
    assertStringsEqual(symbol, "DGB");
    assertStringsEqual(txUrl, "https://digiexplorer.info/tx/t123");
    assertStringsEqual(accUrl, "https://digiexplorer.info/address/a12");
    assertStringsEqual(id, "digibyte");
    assertStringsEqual(name, "DigiByte");
}
