// Copyright © 2017-2024 Trust Wallet.
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


 TEST(TWMAYAChainCoinType, TWCoinType) {
     const auto coin = TWCoinTypeMAYAChain;
     const auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(coin));
     const auto id = WRAPS(TWCoinTypeConfigurationGetID(coin));
     const auto name = WRAPS(TWCoinTypeConfigurationGetName(coin));
     const auto chainId = WRAPS(TWCoinTypeChainId(coin));
     const auto txId = WRAPS(TWStringCreateWithUTF8Bytes("908E8634B357FE5BAB92C0304D3022F9B2A5437A11B459E3C3226EA8E66EC337"));
     const auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(coin, txId.get()));
     const auto accId = WRAPS(TWStringCreateWithUTF8Bytes("maya127ywj4hgff8kskkg5nsa5rkt3vu2gvz3f7m5my"));
     const auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(coin, accId.get()));

     assertStringsEqual(id, "mayachain");
     assertStringsEqual(name, "MAYAChain");
     assertStringsEqual(symbol, "CACAO");
     ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(coin), 10);
     ASSERT_EQ(TWCoinTypeBlockchain(coin), TWBlockchainMayachain);
     ASSERT_EQ(TWCoinTypeP2shPrefix(coin), 0x0);
     ASSERT_EQ(TWCoinTypeStaticPrefix(coin), 0x0);
     assertStringsEqual(chainId, "mayachain-mainnet-v1");
     assertStringsEqual(txUrl, "https://mayascan.org/tx/908E8634B357FE5BAB92C0304D3022F9B2A5437A11B459E3C3226EA8E66EC337");
     assertStringsEqual(accUrl, "https://mayascan.org/address/maya127ywj4hgff8kskkg5nsa5rkt3vu2gvz3f7m5my");
 }
