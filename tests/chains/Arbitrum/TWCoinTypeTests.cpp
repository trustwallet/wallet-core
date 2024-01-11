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


TEST(TWArbitrumCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeArbitrum));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xa1e319be22c08155e5904aa211fb87df5f4ade48de79c6578b1cf3dfda9e498c"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeArbitrum, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xecf9ffa7f51e1194f89c25ad8c484f6bfd04e1ac"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeArbitrum, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeArbitrum));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeArbitrum));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeArbitrum), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeArbitrum));

    assertStringsEqual(symbol, "ETH");
    assertStringsEqual(txUrl, "https://arbiscan.io/tx/0xa1e319be22c08155e5904aa211fb87df5f4ade48de79c6578b1cf3dfda9e498c");
    assertStringsEqual(accUrl, "https://arbiscan.io/address/0xecf9ffa7f51e1194f89c25ad8c484f6bfd04e1ac");
    assertStringsEqual(id, "arbitrum");
    assertStringsEqual(name, "Arbitrum");
}
