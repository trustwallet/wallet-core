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


TEST(TWCrescentCoinType, TWCoinType) {
    const auto coin = TWCoinTypeCrescent;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0C46F4B65706FB5A1FB3A7C32543CF7836DA33EB88295573F66F1886A264E852"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("cre1ekj60f38vatr9fxy4p2t04mwedpc3mc6v38d6n"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "crescent");
    assertStringsEqual(name, "Crescent");
    assertStringsEqual(symbol, "CRE");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "crescent-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/crescent/txs/0C46F4B65706FB5A1FB3A7C32543CF7836DA33EB88295573F66F1886A264E852");
    assertStringsEqual(accUrl, "https://www.mintscan.io/crescent/account/cre1ekj60f38vatr9fxy4p2t04mwedpc3mc6v38d6n");
}
