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


TEST(TWZcashCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcash));
    auto txId = TWStringCreateWithUTF8Bytes("t123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, txId));
    auto accId = TWStringCreateWithUTF8Bytes("a12");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeZcash, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcash));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcash));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcash), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcash));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZcash));
    assertStringsEqual(symbol, "ZEC");
    assertStringsEqual(txUrl, "https://sochain.com/tx/ZEC/t123");
    assertStringsEqual(accUrl, "https://sochain.com/address/ZEC/a12");
    assertStringsEqual(id, "zcash");
    assertStringsEqual(name, "Zcash");
}
