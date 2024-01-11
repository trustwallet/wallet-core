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


TEST(TWEthereumClassicCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x66004165d3901819dc22e568931591d2e4287bda54995f4ce2701a12016f5997"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9eab4b0fc468a7f5d46228bf5a76cb52370d068d"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeEthereumClassic, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));
    const auto chainId = WRAPS(TWCoinTypeChainId(TWCoinTypeEthereumClassic));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereumClassic));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeEthereumClassic));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeEthereumClassic));
    assertStringsEqual(symbol, "ETC");
    assertStringsEqual(txUrl, "https://blockscout.com/etc/mainnet/tx/0x66004165d3901819dc22e568931591d2e4287bda54995f4ce2701a12016f5997");
    assertStringsEqual(accUrl, "https://blockscout.com/etc/mainnet/address/0x9eab4b0fc468a7f5d46228bf5a76cb52370d068d");
    assertStringsEqual(id, "classic");
    assertStringsEqual(name, "Ethereum Classic");
    assertStringsEqual(chainId, "61");
}
