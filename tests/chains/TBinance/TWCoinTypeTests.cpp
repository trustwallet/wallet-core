// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "TestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWTBinanceCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTBinance));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("92E9DA1B6D603667E2DE83C0AC0C1D9E6D65405AD642DA794421C64A82A078D0"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTBinance, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("tbnb1c2cxgv3cklswxlvqr9anm6mlp6536qnd36txgr"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeTBinance, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTBinance));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTBinance));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTBinance), 8);
    ASSERT_EQ(TWBlockchainBinance, TWCoinTypeBlockchain(TWCoinTypeTBinance));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeTBinance));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeTBinance));
    assertStringsEqual(symbol, "BNB");
    assertStringsEqual(txUrl, "https://testnet-explorer.binance.org/tx/92E9DA1B6D603667E2DE83C0AC0C1D9E6D65405AD642DA794421C64A82A078D0");
    assertStringsEqual(accUrl, "https://testnet-explorer.binance.org/address/tbnb1c2cxgv3cklswxlvqr9anm6mlp6536qnd36txgr");
    assertStringsEqual(id, "tbinance");
    assertStringsEqual(name, "TBNB");
}
