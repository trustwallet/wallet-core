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


TEST(TWAcalaEVMCoinType, TWCoinType) {
    const auto coin = TWCoinTypeAcalaEVM;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x4b0b151dd71ed8ef3174da18565790bf14f0a903a13e4f3266c7848bc8841593"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "acalaevm");
    assertStringsEqual(name, "Acala EVM");
    assertStringsEqual(symbol, "ACA");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "787");
    assertStringsEqual(txUrl, "https://blockscout.acala.network/tx/0x4b0b151dd71ed8ef3174da18565790bf14f0a903a13e4f3266c7848bc8841593");
    assertStringsEqual(accUrl, "https://blockscout.acala.network/address/0x9d1d97aDFcd324Bbd603D3872BD78e04098510b1");
}
