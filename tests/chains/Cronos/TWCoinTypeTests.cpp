// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWCronosCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCronosChain));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x850131282053328ad569fa91200aa970cbed7d97b52951fe8b449cca3708789e"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCronosChain, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x44eed2bb80b688a8778173c19fe11cd6876af15a"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCronosChain, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCronosChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCronosChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCronosChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCronosChain));

    assertStringsEqual(symbol, "CRO");
    assertStringsEqual(txUrl, "https://cronoscan.com/tx/0x850131282053328ad569fa91200aa970cbed7d97b52951fe8b449cca3708789e");
    assertStringsEqual(accUrl, "https://cronoscan.com/address/0x44eed2bb80b688a8778173c19fe11cd6876af15a");
    assertStringsEqual(id, "cronos");
    assertStringsEqual(name, "Cronos Chain");
}
