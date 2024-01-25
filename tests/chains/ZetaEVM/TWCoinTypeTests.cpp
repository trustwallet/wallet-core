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


TEST(TWZetaEVMCoinType, TWCoinType) {
    const auto coin = TWCoinTypeZetaEVM;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x04cb1201857de29af97b755e51c888454fb96c1f3bb3c1329bb94d5353d5c19e"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x85539A58F9c88DdDccBaBBfc660968323Fd1e167"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zetaevm");
    assertStringsEqual(name, "Zeta EVM");
    assertStringsEqual(symbol, "ZETA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "7000");
    assertStringsEqual(txUrl, "https://explorer.zetachain.com/evm/tx/0x04cb1201857de29af97b755e51c888454fb96c1f3bb3c1329bb94d5353d5c19e");
    assertStringsEqual(accUrl, "https://explorer.zetachain.com/address/0x85539A58F9c88DdDccBaBBfc660968323Fd1e167");
}
