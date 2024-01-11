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


TEST(TWAlgorandCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAlgorand));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("CR7POXFTYDLC7TV3IXHA7AZKWABUJC52BACLHJQNXAKZJGRPQY3A"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAlgorand, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("J4AEINCSSLDA7LNBNWM4ZXFCTLTOZT5LG3F5BLMFPJYGFWVCMU37EZI2AM"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeAlgorand, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAlgorand));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAlgorand));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAlgorand), 6);
    ASSERT_EQ(TWBlockchainAlgorand, TWCoinTypeBlockchain(TWCoinTypeAlgorand));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeAlgorand));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeAlgorand));
    assertStringsEqual(symbol, "ALGO");
    assertStringsEqual(txUrl, "https://algoexplorer.io/tx/CR7POXFTYDLC7TV3IXHA7AZKWABUJC52BACLHJQNXAKZJGRPQY3A");
    assertStringsEqual(accUrl, "https://algoexplorer.io/address/J4AEINCSSLDA7LNBNWM4ZXFCTLTOZT5LG3F5BLMFPJYGFWVCMU37EZI2AM");
    assertStringsEqual(id, "algorand");
    assertStringsEqual(name, "Algorand");
}
