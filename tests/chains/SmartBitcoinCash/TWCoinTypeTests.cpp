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


TEST(TWSmartBitcoinCashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSmartBitcoinCash));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x6413466b455b17d03c7a8ce2d7f99fec34bcd338628bdd2d0580a21e3197a4d9"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSmartBitcoinCash, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xFeEc227410E1DF9f3b4e6e2E284DC83051ae468F"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeSmartBitcoinCash, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSmartBitcoinCash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSmartBitcoinCash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSmartBitcoinCash), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeSmartBitcoinCash));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeSmartBitcoinCash));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeSmartBitcoinCash));
    assertStringsEqual(symbol, "BCH");
    assertStringsEqual(txUrl, "https://www.smartscan.cash/tx/0x6413466b455b17d03c7a8ce2d7f99fec34bcd338628bdd2d0580a21e3197a4d9");
    assertStringsEqual(accUrl, "https://www.smartscan.cash/address/0xFeEc227410E1DF9f3b4e6e2E284DC83051ae468F");
    assertStringsEqual(id, "smartbch");
    assertStringsEqual(name, "Smart Bitcoin Cash");
}
