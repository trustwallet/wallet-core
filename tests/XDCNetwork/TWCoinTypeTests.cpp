// Copyright © 2017-2020 Trust Wallet.
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


TEST(TWXDCNetworkCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDCNetwork));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x35a8d3ab06c94d5b7d27221b7c9a24ba3f1710dd0fcfd75c5d59b3a885fd709b"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXDCNetwork, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("xdc86cCbD9bfb371c355202086882bC644A7D0b024B"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeXDCNetwork, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXDCNetwork));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXDCNetwork));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXDCNetwork), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeXDCNetwork));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXDCNetwork));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXDCNetwork));
    assertStringsEqual(symbol, "XDC");
    assertStringsEqual(txUrl, "https://xdc.blocksscan.io/txs/0x91bba2a63fc44e501777b9f9bc157de45f14d5bde6ef720fddd6d3cbce6450f9");
    assertStringsEqual(accUrl, "https://xdc.blocksscan.io/address/xdce859abbce60afa0b71f615c3de0a361d867a2cdb");
    assertStringsEqual(id, "xdcnetwork");
    assertStringsEqual(name, "XDCNetwork");
}
