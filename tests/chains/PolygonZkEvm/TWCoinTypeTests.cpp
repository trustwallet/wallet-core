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
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xc70fd1a45b3130f5515a27d96f01a7f508099fb0b8af52ef432d5e4b2373dccd"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x81d98c8fda0410ee3e9d7586cb949cd19fa4cf38"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "polygonzkevm");
    assertStringsEqual(name, "Polygon zkEVM");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    ASSERT_EQ(coin, 10001101ull);
    assertStringsEqual(chainId, "1101");
    assertStringsEqual(txUrl, "https://zkevm.polygonscan.com/tx/0xc70fd1a45b3130f5515a27d96f01a7f508099fb0b8af52ef432d5e4b2373dccd");
    assertStringsEqual(accUrl, "https://zkevm.polygonscan.com/address/0x81d98c8fda0410ee3e9d7586cb949cd19fa4cf38");
}

} // namespace TW::TWZksync::tests
