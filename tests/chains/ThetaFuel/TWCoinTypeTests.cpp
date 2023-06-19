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

TEST(TWThetaFuelCoinType, TWCoinType) {
    const auto coin = TWCoinTypeThetaFuel;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xdb1c1c4e06289a4fc71b98ced218242d4f4a54a09987791a6a53a5260c053555"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xa144e6a98b967e585b214bfa7f6692af81987e5b"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "tfuelevm");
    assertStringsEqual(name, "Theta Fuel");
    assertStringsEqual(symbol, "TFUEL");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "361");
    assertStringsEqual(txUrl, "https://explorer.thetatoken.org/tx/0xdb1c1c4e06289a4fc71b98ced218242d4f4a54a09987791a6a53a5260c053555");
    assertStringsEqual(accUrl, "https://explorer.thetatoken.org/account/0xa144e6a98b967e585b214bfa7f6692af81987e5b");
}
