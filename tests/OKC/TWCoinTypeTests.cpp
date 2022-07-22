// Copyright © 2017-2022 Trust Wallet.
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


TEST(TWOKCCoinType, TWCoinType) {
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOKC));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0x46C3A947E8248570FBD28E4FE456CC8F80DFD90716533878FB67857B95FA3D37"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOKC, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x074faafd0b20fad2efa115b8ed7e75993e580b85"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeOKC, accId.get()));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOKC));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOKC));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOKC), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeOKC));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeOKC));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeOKC));
    assertStringsEqual(symbol, "MTR");
    assertStringsEqual(txUrl, "https://www.oklink.com/en/okc/tx/0x46C3A947E8248570FBD28E4FE456CC8F80DFD90716533878FB67857B95FA3D37");
    assertStringsEqual(accUrl, "https://www.oklink.com/en/okc/address/0x074faafd0b20fad2efa115b8ed7e75993e580b85");
    assertStringsEqual(id, "okc");
    assertStringsEqual(name, "OKC");
}
