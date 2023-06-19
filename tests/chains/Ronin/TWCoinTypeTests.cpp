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


TEST(TWRoninCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRonin));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xc09835aaf9c1cacea8ce322865583c791d3a4dfbd9a3b72f79539db88d3697ab"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRonin, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xdc05ecd5fbdb64058d94f3182d66f44342b9adcb"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeRonin, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRonin));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRonin));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRonin), 18);
    ASSERT_EQ(TWBlockchainRonin, TWCoinTypeBlockchain(TWCoinTypeRonin));

    assertStringsEqual(symbol, "RON");
    assertStringsEqual(txUrl, "https://explorer.roninchain.com/tx/0xc09835aaf9c1cacea8ce322865583c791d3a4dfbd9a3b72f79539db88d3697ab");
    assertStringsEqual(accUrl, "https://explorer.roninchain.com/address/0xdc05ecd5fbdb64058d94f3182d66f44342b9adcb");
    assertStringsEqual(id, "ronin");
    assertStringsEqual(name, "Ronin");
}
