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


TEST(TWCoinTypeBluzelle, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBluzelle));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("AC026E0EC6E33A77D5EA6B9CEF9810699BC2AD8C5582E007E7857457C6D3B819"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBluzelle, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("bluzelle1q9cryfal7u3jvnq6er5ufety20xtzw6ycx2te9"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBluzelle, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBluzelle));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBluzelle));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBluzelle), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeBluzelle));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBluzelle));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBluzelle));
    assertStringsEqual(symbol, "BLZ");
    assertStringsEqual(txUrl, "https://bigdipper.net.bluzelle.com/transactions/AC026E0EC6E33A77D5EA6B9CEF9810699BC2AD8C5582E007E7857457C6D3B819");
    assertStringsEqual(accUrl, "https://bigdipper.net.bluzelle.com/account/bluzelle1q9cryfal7u3jvnq6er5ufety20xtzw6ycx2te9");
    assertStringsEqual(id, "bluzelle");
    assertStringsEqual(name, "Bluzelle");
}
