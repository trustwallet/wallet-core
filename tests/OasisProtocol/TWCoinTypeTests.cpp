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


TEST(TWOasisProtocolCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOasisProtocol));
    auto txId = TWStringCreateWithUTF8Bytes("0b9bd4983f1c88a1c71bf33562b6ba02b3064e01697d15a0de4bfe1922ec74b8");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOasisProtocol, txId));
    auto accId = TWStringCreateWithUTF8Bytes("oasis1qrx376dmwuckmruzn9vq64n49clw72lywctvxdf4");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOasisProtocol, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOasisProtocol));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOasisProtocol));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOasisProtocol), 9);
    ASSERT_EQ(TWBlockchainOasisNetwork, TWCoinTypeBlockchain(TWCoinTypeOasisProtocol));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOasisProtocol));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOasisProtocol));
    assertStringsEqual(symbol, "ROSE");
    assertStringsEqual(txUrl, "https://oasisscan.com/transactions/0b9bd4983f1c88a1c71bf33562b6ba02b3064e01697d15a0de4bfe1922ec74b8");
    assertStringsEqual(accUrl, "https://oasisscan.com/accounts/detail/oasis1qrx376dmwuckmruzn9vq64n49clw72lywctvxdf4");
    assertStringsEqual(id, "Oasis");
    assertStringsEqual(name, "Oasis Protocol");
}
