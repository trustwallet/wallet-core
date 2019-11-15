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


TEST(TWKusamaCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKusama));
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKusama, txId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKusama));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKusama));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKusama), 12);
    ASSERT_EQ(TWBlockchainPolkadot, TWCoinTypeBlockchain(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeKusama));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeKusama));
    assertStringsEqual(symbol, "KSM");
    assertStringsEqual(txUrl, "https://polkascan.io/pre/kusama-cc2/transaction/123");
    assertStringsEqual(id, "kusama");
    assertStringsEqual(name, "Kusama");
}
