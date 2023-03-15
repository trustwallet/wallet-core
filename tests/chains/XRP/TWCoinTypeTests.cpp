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


TEST(TWXRPCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXRP));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("E26AB8F3372D2FC02DEC1FD5674ADAB762D684BFFDBBDF5D674E9D7CF4A47054"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXRP, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("rfkH7EuS1XcSkB9pocy1R6T8F4CsNYixYU"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeXRP, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXRP));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXRP), 6);
    ASSERT_EQ(TWBlockchainRipple, TWCoinTypeBlockchain(TWCoinTypeXRP));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeXRP));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeXRP));
    assertStringsEqual(symbol, "XRP");
    assertStringsEqual(txUrl, "https://bithomp.com/explorer/E26AB8F3372D2FC02DEC1FD5674ADAB762D684BFFDBBDF5D674E9D7CF4A47054");
    assertStringsEqual(accUrl, "https://bithomp.com/explorer/rfkH7EuS1XcSkB9pocy1R6T8F4CsNYixYU");
    assertStringsEqual(id, "ripple");
    assertStringsEqual(name, "XRP");
}
