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
    TEST(TWTiaCoinType, TWCoinType) {
        const auto coin = TWCoinTypeTia;
        const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
        const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
        const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
        const auto chainId = WRAPS(TWCoinTypeChainId(coin));
        const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("943B1A59EB724A1258CF167834A83558DE5A31E8C1E31FCBCBB99404222E5FBF"));
        const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
        const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("celestia142j9u5eaduzd7faumygud6ruhdwme98qpwmfv7"));
        const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));
        assertStringsEqual(id, "tia");
        assertStringsEqual(name, "Celestia");
        assertStringsEqual(symbol, "TIA");
        ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 6);
        ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainCosmos);
        ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
        ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
        assertStringsEqual(chainId, "mocha-4");
        assertStringsEqual(txUrl, "https://www.mintscan.io/tia/txs/943B1A59EB724A1258CF167834A83558DE5A31E8C1E31FCBCBB99404222E5FBF");
        assertStringsEqual(accUrl, "https://www.mintscan.io/tia/account/celestia142j9u5eaduzd7faumygud6ruhdwme98qpwmfv7");
    }
}
