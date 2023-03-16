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


TEST(TWCoinTypeOKXChain, TWCoinType) {
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOKXChain));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x46C3A947E8248570FBD28E4FE456CC8F80DFD90716533878FB67857B95FA3D37"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOKXChain, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x074faafd0b20fad2efa115b8ed7e75993e580b85"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOKXChain, accId.get()));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOKXChain));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOKXChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOKXChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeOKXChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOKXChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOKXChain));
    assertStringsEqual(symbol, "OKT");
    assertStringsEqual(txUrl, "https://www.oklink.com/en/okc/tx/0x46C3A947E8248570FBD28E4FE456CC8F80DFD90716533878FB67857B95FA3D37");
    assertStringsEqual(accUrl, "https://www.oklink.com/en/okc/address/0x074faafd0b20fad2efa115b8ed7e75993e580b85");
    assertStringsEqual(id, "okc");
    assertStringsEqual(name, "OKX Chain");
}
