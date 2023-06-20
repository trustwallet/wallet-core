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


TEST(TWCeloCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCelo));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xaf41ee58afe633dc7b179c15693cca40fe0372c1d7c70351620105d59d326693"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCelo, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0xFBFf95e2Fa7e4Ff3aeA34eFB05fB60F9968a6aaD"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCelo, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCelo));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCelo));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCelo), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCelo));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCelo));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCelo));
    assertStringsEqual(symbol, "CELO");
    assertStringsEqual(txUrl, "https://explorer.celo.org/tx/0xaf41ee58afe633dc7b179c15693cca40fe0372c1d7c70351620105d59d326693");
    assertStringsEqual(accUrl, "https://explorer.celo.org/address/0xFBFf95e2Fa7e4Ff3aeA34eFB05fB60F9968a6aaD");
    assertStringsEqual(id, "celo");
    assertStringsEqual(name, "Celo");
}
