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

namespace TW::Cosmos::tests {
    TEST(TWQuasarCoinType, TWCoinType) {
        const auto coin = TWCoinTypeQuasar;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("2898B89C98FE1E8CF1E05A37E4EE5EE5ED83FD957B0CAEE53DE39FC82BF1A033"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("quasar1cqu6w425slheul3jsmyt6q0ec0rs0w0ugkst3k"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

        assertStringsEqual(id, "quasar");
        assertStringsEqual(name, "Quasar");
        assertStringsEqual(symbol, "QSR");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "quasar-1");
        assertStringsEqual(txUrl, "https://www.mintscan.io/quasar/txs/2898B89C98FE1E8CF1E05A37E4EE5EE5ED83FD957B0CAEE53DE39FC82BF1A033");
        assertStringsEqual(accUrl, "https://www.mintscan.io/quasar/account/quasar1cqu6w425slheul3jsmyt6q0ec0rs0w0ugkst3k");
    }
}
