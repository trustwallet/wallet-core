// Copyright © 2017-2022 Trust Wallet.
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


TEST(TWVergeCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVerge));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("8c99979a2b25a46659bff35b238aab1c3158f736f215d99526429c7c96203581"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVerge, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("DFre88gd87bAZQdnS7dbBLwT6GWiGFMQB6"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeVerge, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVerge));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVerge));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVerge), 6);
    ASSERT_EQ(TWBlockchainVerge, TWCoinTypeBlockchain(TWCoinTypeVerge));
    ASSERT_EQ(0x21, TWCoinTypeP2shPrefix(TWCoinTypeVerge));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeVerge));
    assertStringsEqual(symbol, "XVG");
    assertStringsEqual(txUrl, "https://verge-blockchain.info/tx/8c99979a2b25a46659bff35b238aab1c3158f736f215d99526429c7c96203581");
    assertStringsEqual(accUrl, "https://verge-blockchain.info/address/DFre88gd87bAZQdnS7dbBLwT6GWiGFMQB6");
    assertStringsEqual(id, "verge");
    assertStringsEqual(name, "Verge");
}
