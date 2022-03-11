// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWConfluxCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeConflux));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x702ba9ba3c283c17df5d96b16505833ef5e25d3a5bba2eed8b69a3ac0482ae98"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeConflux, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x3eecaa466e87a46b737aeedfe951bcc8403a4e1e"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeConflux, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeConflux));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeConflux));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeConflux), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeConflux));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeConflux));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeConflux));
    assertStringsEqual(symbol, "CFX");
    assertStringsEqual(txUrl, "https://evm.confluxscan.net/tx/0x702ba9ba3c283c17df5d96b16505833ef5e25d3a5bba2eed8b69a3ac0482ae98");
    assertStringsEqual(accUrl, "https://evm.confluxscan.net/address/0xFeEc227410E1DF9f3b4e6e2E284DC83051ae468F");
    assertStringsEqual(id, "conflux");
    assertStringsEqual(name, "Conflux eSpace");
}
