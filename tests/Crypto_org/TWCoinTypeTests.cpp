// Copyright © 2017-2021 Trust Wallet.
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


TEST(TWCrypto_orgCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCrypto_org));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("6B9CD39A454F187B7BC5AFE10DC5F8D19A0C50A20A1AABEEE2D5A87B2B62190A"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCrypto_org, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("cro1yjjlx5qsrj5rxn5xtd5rkm6dcqzlchxkrvsmg6"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeCrypto_org, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCrypto_org));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCrypto_org));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCrypto_org), 8);
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCrypto_org));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeCrypto_org));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeCrypto_org));
    assertStringsEqual(symbol, "CRO");
    assertStringsEqual(txUrl, "https://crypto.org/explorer/txs/6B9CD39A454F187B7BC5AFE10DC5F8D19A0C50A20A1AABEEE2D5A87B2B62190A");
    assertStringsEqual(accUrl, "https://crypto.org/explorer/account/cro1yjjlx5qsrj5rxn5xtd5rkm6dcqzlchxkrvsmg6");
    assertStringsEqual(id, "cryptoorg");
    assertStringsEqual(name, "Crypto_org");
}
