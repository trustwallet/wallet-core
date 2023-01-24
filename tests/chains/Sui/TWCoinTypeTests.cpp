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


TEST(TWSuiCoinType, TWCoinType) {
    const auto coin = TWCoinTypeSui;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("SWRW1RoMHxnD9NeobgBoC4cXGwp2Hc511CnfWUoTBmo"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x62107e1afefccc7b2267ab74e332c146f5c2ca15"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "sui");
    assertStringsEqual(name, "Sui");
    assertStringsEqual(symbol, "SUI");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 9);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainSui);
    assertStringsEqual(txUrl, "https://explorer.sui.io//transaction/SWRW1RoMHxnD9NeobgBoC4cXGwp2Hc511CnfWUoTBmo");
    assertStringsEqual(accUrl, "https://explorer.sui.io//address/0x62107e1afefccc7b2267ab74e332c146f5c2ca15");
}
