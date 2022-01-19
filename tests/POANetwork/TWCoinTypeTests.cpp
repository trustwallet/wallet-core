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


TEST(TWPOANetworkCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOA));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("t123"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOA, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("a12"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypePOA, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOA));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOA));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePOA), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePOA));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypePOA));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypePOA));
    assertStringsEqual(symbol, "POA");
    assertStringsEqual(txUrl, "https://blockscout.com/poa/core/tx/t123");
    assertStringsEqual(accUrl, "https://blockscout.com/poa/core/address/a12");
    assertStringsEqual(id, "poa");
    assertStringsEqual(name, "POA Network");
}
