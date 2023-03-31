// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

namespace TW::TWZksync::tests {

TEST(TWZksyncCoinType, TWCoinType) {
    const auto coin = TWCoinTypeZksync;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xae38d3ede1104d088b474da261d0eb4847952c3db24c21e820502f4c1b0c01f5"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xeF86b2c8740518548ae449c4C3892B4be0475d8c"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zksync");
    assertStringsEqual(name, "zkSync Era");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeZksync, 10000324ull);
    assertStringsEqual(chainId, "324");
    assertStringsEqual(txUrl, "https://explorer.zksync.io/tx/0xae38d3ede1104d088b474da261d0eb4847952c3db24c21e820502f4c1b0c01f5");
    assertStringsEqual(accUrl, "https://explorer.zksync.io/address/0xeF86b2c8740518548ae449c4C3892B4be0475d8c");
}

} // namespace TW::TWZksync::tests
