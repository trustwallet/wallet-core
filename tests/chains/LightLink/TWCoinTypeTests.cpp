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


TEST(TWLightLinkCoinType, TWCoinType) {
    const auto coin = TWCoinTypeLightLink;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x11c250e1ba9a94c64d241a2ed6c0f39407fa9898c5aa2a176f38c31f04469877"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xCD3b6008ba2F5115004ab7A07Be779cAa63CdF3F"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "lightlink");
    assertStringsEqual(name, "LightLink");
    assertStringsEqual(symbol, "LL");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1890");
    assertStringsEqual(txUrl, "https://phoenix.lightlink.io/tx/0x11c250e1ba9a94c64d241a2ed6c0f39407fa9898c5aa2a176f38c31f04469877");
    assertStringsEqual(accUrl, "https://phoenix.lightlink.io/address/0xCD3b6008ba2F5115004ab7A07Be779cAa63CdF3F");
}
