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


TEST(TWCosmosCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("541FA06FB37AC1BF61922143783DD76FECA361830F9876D0342536EE8A87A790"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCosmos, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("cosmos1gu6y2a0ffteesyeyeesk23082c6998xyzmt9mz"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCosmos, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCosmos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCosmos));
    const auto chainId = WRAPS(TWCoinTypeChainId(TWCoinTypeCosmos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCosmos), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCosmos));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCosmos));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCosmos));
    assertStringsEqual(chainId, "cosmoshub-4");
    assertStringsEqual(symbol, "ATOM");
    assertStringsEqual(txUrl, "https://mintscan.io/cosmos/txs/541FA06FB37AC1BF61922143783DD76FECA361830F9876D0342536EE8A87A790");
    assertStringsEqual(accUrl, "https://mintscan.io/cosmos/account/cosmos1gu6y2a0ffteesyeyeesk23082c6998xyzmt9mz");
    assertStringsEqual(id, "cosmos");
    assertStringsEqual(name, "Cosmos Hub");
}
