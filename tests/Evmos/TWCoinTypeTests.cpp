// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>

TEST(TWEvmosCoinType, TWCoinTypeEvmos) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEvmos));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x24af42cf4977a96d62e3a82c3cd9b519c3e7c53dd83398b88f0cb435d867b422"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEvmos, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x30627903124Aa1e71384bc52e1cb96E4AB3252b6"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeEvmos, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEvmos));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEvmos));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEvmos), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEvmos));

    assertStringsEqual(symbol, "EVMOS");
    assertStringsEqual(txUrl, "https://evm.evmos.org/tx/0x24af42cf4977a96d62e3a82c3cd9b519c3e7c53dd83398b88f0cb435d867b422");
    assertStringsEqual(accUrl, "https://evm.evmos.org/address/0x30627903124Aa1e71384bc52e1cb96E4AB3252b6");
    assertStringsEqual(id, "evmos");
    assertStringsEqual(name, "Evmos");
}

TEST(TWEvmosCoinType, TWCoinTypeEvmosZone) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEvmosZone));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEvmosZone, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeEvmosZone, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEvmosZone));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEvmosZone));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEvmosZone), 18);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeEvmosZone));

    assertStringsEqual(symbol, "EVMOS");
    assertStringsEqual(txUrl, "https://mintscan.io/evmos/txs/A16C211C83AD1E684DE46F694FAAC17D8465C864BD7385A81EC062CDE0638811");
    assertStringsEqual(accUrl, "https://mintscan.io/evmos/account/evmos17xpfvakm2amg962yls6f84z3kell8c5ljcjw34");
    assertStringsEqual(id, "evmoszone");
    assertStringsEqual(name, "Evmos Zone");
}
