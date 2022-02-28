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


TEST(TWElrondCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeElrond));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("1fc9785cb8bea0129a16cf7bddc97630c176a556ea566f0e72923c882b5cb3c8"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeElrond, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("erd12yne790km8ezwetkz7m3hmqy9utdc6vdkgsunfzpwguec6v04p2qtk9uqj"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeElrond, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeElrond));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeElrond));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeElrond), 18);
    ASSERT_EQ(TWBlockchainElrondNetwork, TWCoinTypeBlockchain(TWCoinTypeElrond));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeElrond));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeElrond));
    assertStringsEqual(symbol, "eGLD");
    assertStringsEqual(txUrl, "https://explorer.elrond.com/transactions/1fc9785cb8bea0129a16cf7bddc97630c176a556ea566f0e72923c882b5cb3c8");
    assertStringsEqual(accUrl, "https://explorer.elrond.com/address/erd12yne790km8ezwetkz7m3hmqy9utdc6vdkgsunfzpwguec6v04p2qtk9uqj");
    assertStringsEqual(id, "elrond");
    assertStringsEqual(name, "Elrond");
}
