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


TEST(TWIOVCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIOV));
    auto txId = TWStringCreateWithUTF8Bytes("5E189F10BE129169096096B6AD39547631780E31089B1C56C1DB35C92FD39801");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIOV, txId));
    auto accId = TWStringCreateWithUTF8Bytes("star1mj92w3zhd89znd26qq6yarpr86yc573khrm0hv");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeIOV, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIOV));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIOV));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIOV), 6);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeIOV));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeIOV));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeIOV));
    assertStringsEqual(symbol, "IOV");
    assertStringsEqual(txUrl, "https://big-dipper.iov-mainnet-2.iov.one/transactions/5E189F10BE129169096096B6AD39547631780E31089B1C56C1DB35C92FD39801");
    assertStringsEqual(accUrl, "https://big-dipper.iov-mainnet-2.iov.one/account/star1mj92w3zhd89znd26qq6yarpr86yc573khrm0hv");
    assertStringsEqual(id, "iov");
    assertStringsEqual(name, "IOV");
}

