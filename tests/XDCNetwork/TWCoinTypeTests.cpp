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


TEST(TWXDCNetworkCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDCNetwork));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXDCNetwork, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("xdcbc4f1b0c59857dd97089d9d860f169d712c1fb6e"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeXDCNetwork, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXDCNetwork));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXDCNetwork));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXDCNetwork), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeXDCNetwork));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXDCNetwork));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXDCNetwork));
    assertStringsEqual(symbol, "XDC");
    assertStringsEqual(txUrl, "https://xdc.blocksscan.io/tx/0x5c4fa942ff0b39651e5ffd21d646f2956a289ce9f26a59ddbef5dfa27701aa56");
    assertStringsEqual(accUrl, "https://xdc.blocksscan.io/address/xdcbc4f1b0c59857dd97089d9d860f169d712c1fb6e");
    assertStringsEqual(id, "xdc");
    assertStringsEqual(name, "XDC Network");
}
