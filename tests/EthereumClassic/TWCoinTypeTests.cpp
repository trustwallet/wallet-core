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


TEST(TWEthereumClassicCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereumClassic));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeEthereumClassic));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeEthereumClassic));
    assertStringsEqual(symbol, "ETC");
    assertStringsEqual(txUrl, "https://gastracker.io/tx/123");
    assertStringsEqual(id, "classic");
    assertStringsEqual(name, "Ethereum Classic");
}
