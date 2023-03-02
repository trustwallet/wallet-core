// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::TWPolygonZkEvm::tests {

TEST(TWPolygonZkEVMCoinType, TWCoinType) {
    const auto coin = TWCoinTypePolygonzkEVM;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xfbe568183495ad2ec95f0232501e86018bff3e4ca52aaa804983b380dc39d523"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xC5C39Ec2a271d84d67c0e66a68238102A2ec4b98"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "polygonzkevm");
    assertStringsEqual(name, "Polygon zkEVM");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "1422");
    assertStringsEqual(txUrl, "https://explorer.public.zkevm-test.net/tx/0xfbe568183495ad2ec95f0232501e86018bff3e4ca52aaa804983b380dc39d523");
    assertStringsEqual(accUrl, "https://explorer.public.zkevm-test.net/address/0xC5C39Ec2a271d84d67c0e66a68238102A2ec4b98");
}

} // namespace TW::TWZksync::tests
