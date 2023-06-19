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


TEST(TWFantomCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFantom));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb0a741d882291951de1fac72e90b9baf886ddb0c9c87658a0c206490dfaa5202"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFantom, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9474feb9917b87da6f0d830ba66ee0035835c0d3"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeFantom, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFantom));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFantom));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFantom), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeFantom));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeFantom));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeFantom));
    assertStringsEqual(symbol, "FTM");
    assertStringsEqual(txUrl, "https://ftmscan.com/tx/0xb0a741d882291951de1fac72e90b9baf886ddb0c9c87658a0c206490dfaa5202");
    assertStringsEqual(accUrl, "https://ftmscan.com/address/0x9474feb9917b87da6f0d830ba66ee0035835c0d3");
    assertStringsEqual(id, "fantom");
    assertStringsEqual(name, "Fantom");
}
