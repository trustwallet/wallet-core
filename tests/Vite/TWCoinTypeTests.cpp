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


TEST(TWViteCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVite));
    auto txId = WRAPS(TWStringCreateWithUTF8Bytes("C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F"));
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVite, txId.get()));
    auto accId = WRAPS(TWStringCreateWithUTF8Bytes("vite_1wpj616kwhe1y38y1mspd8aub8i334cwybqco511iyuxm55zx8d67ptf1tsf"));
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeVite, accId.get()));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVite));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVite));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVite), 30);
    ASSERT_EQ(TWBlockchainVite, TWCoinTypeBlockchain(TWCoinTypeVite));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeVite));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeVite));
    assertStringsEqual(symbol, "VITE");
    assertStringsEqual(txUrl, "https://vitescan.io/block/C264DB7BF40738F0CEFF19B606746CB925B713E4B8699A055699E0DC8ABBC70F");
    assertStringsEqual(accUrl, "https://vitescan.io/address/vite_1wpj616kwhe1y38y1mspd8aub8i334cwybqco511iyuxm55zx8d67ptf1tsf");
    assertStringsEqual(id, "vite");
    assertStringsEqual(name, "Vite");
}
