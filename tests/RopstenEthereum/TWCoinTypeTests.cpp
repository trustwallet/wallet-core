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


TEST(TWRopstenCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRopsten));
    auto txId = TWStringCreateWithUTF8Bytes("0xdfe52ecc714b313df82f7fafc49085f8241e0633473009826f5e55afd258b371");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRopsten, txId));
    auto accId = TWStringCreateWithUTF8Bytes("0x81b7e08f65bdf5648606c89998a9cc8164397647");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeRopsten, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRopsten));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRopsten));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRopsten), 18);
    ASSERT_EQ(TWBlockchainRopsten, TWCoinTypeBlockchain(TWCoinTypeRopsten));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeRopsten));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeRopsten));
    assertStringsEqual(symbol, "ETH");
    assertStringsEqual(txUrl, "https://ropsten.etherscan.io/tx/0xdfe52ecc714b313df82f7fafc49085f8241e0633473009826f5e55afd258b371");
    assertStringsEqual(accUrl, "https://ropsten.etherscan.io/address/0x81b7e08f65bdf5648606c89998a9cc8164397647");
    assertStringsEqual(id, "ropsten");
    assertStringsEqual(name, "Ropsten Ethereum");
}
