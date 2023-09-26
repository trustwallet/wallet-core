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


TEST(TWGreenfieldCoinType, TWCoinType) {
    const auto coin = TWCoinTypeGreenfield;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("9F895CF2DD64FB1F428CEFCF2A6585A813C3540FC9FE1EF42DB1DA2CB1DF55AB"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9d1d97adfcd324bbd603d3872bd78e04098510b1"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "greenfield");
    assertStringsEqual(name, "BNB Greenfield");
    assertStringsEqual(symbol, "BNB");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainGreenfield);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "5600");
    assertStringsEqual(txUrl, "https://greenfieldscan.com/tx/9F895CF2DD64FB1F428CEFCF2A6585A813C3540FC9FE1EF42DB1DA2CB1DF55AB");
    assertStringsEqual(accUrl, "https://greenfieldscan.com/account/0x9d1d97adfcd324bbd603d3872bd78e04098510b1");
}
