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

namespace TW::TWZkSyncV2::tests {

TEST(TWZkSyncV2CoinType, TWCoinType) {
    const auto coin = TWCoinTypeZkSyncV2;
    const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
    const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
    const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
    const auto chainId = WRAPS(TWCoinTypeChainId(coin));
    const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("0xb526861291c0335435e3c976e672a464b70762e54d7167409fb4f66e374ed708"));
    const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
    const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("0x970978989a51790ee591b2a54f92c7cd9cdc2f88"));
    const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

    assertStringsEqual(id, "zksyncv2");
    assertStringsEqual(name, "ZkSync v2");
    assertStringsEqual(symbol, "ETH");
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 18);
    ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainEthereum);
    ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
    ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
    assertStringsEqual(chainId, "10");
    assertStringsEqual(txUrl, "https://zksync2-testnet.zkscan.io/tx/0xb526861291c0335435e3c976e672a464b70762e54d7167409fb4f66e374ed708");
    assertStringsEqual(accUrl, "https://zksync2-testnet.zkscan.io/address/0x970978989a51790ee591b2a54f92c7cd9cdc2f88");
}

} // namespace TW::TWZkSyncV2::tests
