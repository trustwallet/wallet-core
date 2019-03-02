// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <gtest/gtest.h>

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetSymbol) {
    auto value1 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereum));
    assertStringsEqual(value1, "ETH");

    auto value2 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoin));
    assertStringsEqual(value2, "BTC");

    auto value3 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinCash));
    assertStringsEqual(value3, "BCH");

    auto value4 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    assertStringsEqual(value4, "CLO");

    auto value5 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDash));
    assertStringsEqual(value5, "DASH");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    assertStringsEqual(value6, "ETC");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGo));
    assertStringsEqual(value7, "GO");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    assertStringsEqual(value8, "ICX");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "LTC");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePoa));
    assertStringsEqual(value10, "POA");

    auto value12 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    assertStringsEqual(value12, "TT");

    auto value13 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    assertStringsEqual(value13, "TOMO");

    auto value14 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    assertStringsEqual(value14, "TRX");

    auto value15 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    assertStringsEqual(value15, "VET");

    auto value16 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    assertStringsEqual(value16, "XZC");

    auto value17 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    assertStringsEqual(value17, "BNB");

    auto value18 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(value18, "EOS");

    auto value19 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(value19, "XTZ");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetDecimals) {
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGo), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeICON), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLitecoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePoa), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinance), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEOS), 0);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetTransactionURL) {
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto value1 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereum, txId));
    assertStringsEqual(value1, "https://etherscan.io/tx/123");

    auto value2 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoin, txId));
    assertStringsEqual(value2, "https://blockchair.com/bitcoin/transaction/123");

    auto value3 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinCash, txId));
    assertStringsEqual(value3, "https://blockchair.com/bitcoin-cash/transaction/123");

    auto value4 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId));
    assertStringsEqual(value4, "https://explorer2.callisto.network/tx/123");

    auto value5 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDash, txId));
    assertStringsEqual(value5, "https://live.blockcypher.com/dash/tx/123");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    assertStringsEqual(value6, "https://gastracker.io/tx/123");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGo, txId));
    assertStringsEqual(value7, "https://explorer.gochain.io/tx/123");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    assertStringsEqual(value8, "https://tracker.icon.foundation/transaction/123");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    assertStringsEqual(value9, "https://blockchair.com/litecoin/transaction/123");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePoa, txId));
    assertStringsEqual(value10, "https://poaexplorer.com/txid/search/123");

    auto value11 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId));
    assertStringsEqual(value11, "https://scan.thundercore.com/transactions/123");

    auto value12 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId));
    assertStringsEqual(value12, "https://scan.tomochain.com/txs/123");

    auto value13 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    assertStringsEqual(value13, "https://tronscan.org/#/transaction/123");

    auto value14 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    assertStringsEqual(value14, "https://tronscan.org/#/transaction/123");

    auto value15 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId));
    assertStringsEqual(value15, "https://explore.veforge.com/transactions/123");

    auto value16 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanChain, txId));
    assertStringsEqual(value16, "https://explorer.wanchain.org/tx/123");

    auto value17 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    assertStringsEqual(value17, "https://explorer.zcoin.io/tx/123");

    auto value18 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    assertStringsEqual(value18, "https://binance.com");

    auto value19 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEOS, txId));
    assertStringsEqual(value19, "https://eospark.com/tx/123");

    auto zecTxId = TWStringCreateWithUTF8Bytes("d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");
    auto value20 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, zecTxId));
    assertStringsEqual(value20, "https://chain.so/tx/ZEC/d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");
    
    auto value21 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    assertStringsEqual(value21, "https://tzscan.io/123");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetID) {
    auto value1 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereum));
    assertStringsEqual(value1, "ethereum");

    auto value2 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoin));
    assertStringsEqual(value2, "bitcoin");

    auto value3 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinCash));
    assertStringsEqual(value3, "bitcoincash");

    auto value4 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    assertStringsEqual(value4, "callisto");

    auto value5 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDash));
    assertStringsEqual(value5, "dash");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    assertStringsEqual(value6, "classic");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGo));
    assertStringsEqual(value7, "gochain");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    assertStringsEqual(value8, "icon");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "litecoin");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePoa));
    assertStringsEqual(value10, "poa");

    auto value12 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    assertStringsEqual(value12, "thundertoken");

    auto value13 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    assertStringsEqual(value13, "tomochain");

    auto value14 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    assertStringsEqual(value14, "tron");

    auto value15 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    assertStringsEqual(value15, "vechain");

    auto value16 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    assertStringsEqual(value16, "zcoin");

    auto value17 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinance));
    assertStringsEqual(value17, "binance");

    auto value18 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEOS));
    assertStringsEqual(value18, "eos");
    
    auto value19 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    assertStringsEqual(value19, "tezos");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetName) {
    auto value1 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereum));
    assertStringsEqual(value1, "Ethereum");

    auto value2 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoin));
    assertStringsEqual(value2, "Bitcoin");

    auto value3 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinCash));
    assertStringsEqual(value3, "Bitcoin Cash");

    auto value4 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));
    assertStringsEqual(value4, "Callisto");

    auto value5 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDash));
    assertStringsEqual(value5, "Dash");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));
    assertStringsEqual(value6, "Ethereum Classic");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGo));
    assertStringsEqual(value7, "GoChain");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));
    assertStringsEqual(value8, "ICON");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "Litecoin");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePoa));
    assertStringsEqual(value10, "POA Network");

    auto value12 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));
    assertStringsEqual(value12, "Thunder Token");

    auto value13 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));
    assertStringsEqual(value13, "TomoChain");

    auto value14 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));
    assertStringsEqual(value14, "Tron");

    auto value15 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));
    assertStringsEqual(value15, "VeChain");

    auto value16 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));
    assertStringsEqual(value16, "Zcoin");

    auto value17 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinance));
    assertStringsEqual(value17, "Binance");

    auto value18 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEOS));
    assertStringsEqual(value18, "EOS");
    
    auto value19 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));
    assertStringsEqual(value19, "Tezos");
}