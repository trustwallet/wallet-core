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


TEST(TWKujiraCoinType, TWCoinType) {
    const auto coin = TWCoinTypeKujira;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("2F5D1B1E0041A86B0590AAD2ED028693E93137A3EA1E614D59FE9B02261BC235"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("kujira13c90ger64wc26s8399rvazceqy273u3n84kgyp"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "kujira");
    assertStringsEqual(name, "Kujira");
    assertStringsEqual(symbol, "KUJI");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "kaiyo-1");
    assertStringsEqual(txUrl, "https://www.mintscan.io/kujira/txs/2F5D1B1E0041A86B0590AAD2ED028693E93137A3EA1E614D59FE9B02261BC235");
    assertStringsEqual(accUrl, "https://www.mintscan.io/kujira/account/kujira13c90ger64wc26s8399rvazceqy273u3n84kgyp");
}
