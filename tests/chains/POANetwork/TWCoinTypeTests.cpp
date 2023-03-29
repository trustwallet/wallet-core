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


TEST(TWPOANetworkCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOANetwork));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOANetwork, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePOANetwork, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOANetwork));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOANetwork));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePOANetwork), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePOANetwork));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePOANetwork));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePOANetwork));
    assertStringsEqual(symbol, "POA");
    assertStringsEqual(txUrl, "https://blockscout.com/poa/core/tx/t123");
    assertStringsEqual(accUrl, "https://blockscout.com/poa/core/address/a12");
    assertStringsEqual(id, "poa");
    assertStringsEqual(name, "POA Network");
}
