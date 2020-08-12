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


TEST(TWBinanceSmartChainCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinanceSmartChain));
    auto txId = TWStringCreateWithUTF8Bytes("0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinanceSmartChain, txId));
    auto accId = TWStringCreateWithUTF8Bytes("0x35552c16704d214347f29Fa77f77DA6d75d7C752");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeBinanceSmartChain, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinanceSmartChain));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinanceSmartChain));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinanceSmartChain), 18);
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeBinanceSmartChain));
    ASSERT_EQ(0x0, TWCoinTypeP2shPrefix(TWCoinTypeBinanceSmartChain));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeBinanceSmartChain));
    assertStringsEqual(symbol, "BNB");
    assertStringsEqual(txUrl, "https://explorer.binance.org/smart-testnet/tx/0xb9ae2e808fe8e57171f303ad8f6e3fd17d949b0bfc7b4db6e8e30a71cc517d7e");
    assertStringsEqual(accUrl, "https://explorer.binance.org/smart-testnet/address/0x35552c16704d214347f29Fa77f77DA6d75d7C752");
    assertStringsEqual(id, "binance-smart");
    assertStringsEqual(name, "Binance SmartChain");
}
