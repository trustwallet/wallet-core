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

    auto decred = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDecred));
    assertStringsEqual(decred, "DCR");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    assertStringsEqual(value6, "ETC");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGoChain));
    assertStringsEqual(value7, "GO");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    assertStringsEqual(value8, "ICX");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "LTC");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOANetwork));
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

    auto value18 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(value18, "XTZ");

    auto value19 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    assertStringsEqual(value19, "XLM");

    auto value20 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDai));
    assertStringsEqual(value20, "xDAI");

    auto value21 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAion));
    assertStringsEqual(value21, "AION");

    auto value22 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    assertStringsEqual(value22, "KIN");

    auto value23 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    assertStringsEqual(value23, "ATOM");

    auto value24 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    assertStringsEqual(value24, "THETA");

    auto value25 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    assertStringsEqual(value25, "GRS");

    auto value26 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLux));
    assertStringsEqual(value26, "LUX");

    auto value27 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    assertStringsEqual(value27, "QTUM");

    auto value28 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthersocial));
    assertStringsEqual(value28, "ESN");

    auto value29 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIocoin));
    assertStringsEqual(value29, "IOC");

    auto value30 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    assertStringsEqual(value30, "BRAVO");

    auto value31 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    assertStringsEqual(value31, "STEEM");

    auto value32 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(value32, "EOS");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetDecimals) {
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDecred), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthersocial), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGoChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGroestlcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeICON), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIocoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLitecoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePOANetwork), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanchain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinance), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXDai), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeStellar), 7);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAion), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKin), 5);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCosmos), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTheta), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLux), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeQtum), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBravoCoin), 3);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSteem), 3);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEOS), 4);
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
    assertStringsEqual(value5, "https://blockchair.com/dash/transaction/123");

    auto value6 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    assertStringsEqual(value6, "https://gastracker.io/tx/123");

    auto value7 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGoChain, txId));
    assertStringsEqual(value7, "https://explorer.gochain.io/tx/123");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    assertStringsEqual(value8, "https://tracker.icon.foundation/transaction/123");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    assertStringsEqual(value9, "https://blockchair.com/litecoin/transaction/123");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOANetwork, txId));
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

    auto value16 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId));
    assertStringsEqual(value16, "https://explorer.wanchain.org/tx/123");

    auto value17 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    assertStringsEqual(value17, "https://explorer.zcoin.io/tx/123");

    auto value18 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    assertStringsEqual(value18, "https://explorer.binance.org/tx/123");

    auto zecTxId = TWStringCreateWithUTF8Bytes("d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");
    auto value20 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, zecTxId));
    assertStringsEqual(value20, "https://chain.so/tx/ZEC/d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");

    auto value21 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    assertStringsEqual(value21, "https://tzscan.io/123");

    auto value22 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeStellar, txId));
    assertStringsEqual(value22, "https://stellarscan.io/transaction/123");

    auto value23 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXDai, txId));
    assertStringsEqual(value23, "https://blockscout.com/poa/dai/tx/123");

    auto value24 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAion, txId));
    assertStringsEqual(value24, "https://mainnet.aion.network/#/transaction/123");

    auto value25 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKin, txId));
    assertStringsEqual(value25, "https://kinexplorer.com/tx/123");

    auto value26 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCosmos, txId));
    assertStringsEqual(value26, "https://hubble.figment.network/chains/cosmoshub-1/blocks/1/transactions/123");

    auto value27 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTheta, txId));
    assertStringsEqual(value27, "https://explorer.thetatoken.org/txs/123");

    auto value28 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGroestlcoin, txId));
    assertStringsEqual(value28, "https://blockbook.groestlcoin.org/tx/123");

    auto value29 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDogecoin, txId));
    assertStringsEqual(value29, "https://live.blockcypher.com/doge/tx/123");

    auto value30 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLux, txId));
    assertStringsEqual(value30, "https://explorer.poswallet.io/#/tx/123");

    auto value31 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId));
    assertStringsEqual(value31, "https://qtum.info/tx/123");

    auto value32 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthersocial, txId));
    assertStringsEqual(value32, "https://ethersocial.net/tx/123");

    auto value33 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIocoin, txId));
    assertStringsEqual(value33, "https://blockbook.iocoin.io/tx/123");

    auto value34 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBravoCoin, txId));
    assertStringsEqual(value34, "https://explorer.bravocoin.com/txid/123");

    auto value35 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSteem, txId));
    assertStringsEqual(value35, "https://steemblockexplorer.com/tx/123");

    auto value36 = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEOS, txId));
    assertStringsEqual(value36, "https://bloks.io/transaction/123");
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

    auto value7 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGoChain));
    assertStringsEqual(value7, "gochain");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    assertStringsEqual(value8, "icon");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "litecoin");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOANetwork));
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

    auto value18 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    assertStringsEqual(value18, "tezos");

    auto value19 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeStellar));
    assertStringsEqual(value19, "stellar");

    auto value20 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXDai));
    assertStringsEqual(value20, "xdai");

    auto value21 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAion));
    assertStringsEqual(value21, "aion");

    auto value22 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKin));
    assertStringsEqual(value22, "kin");

    auto value23 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCosmos));
    assertStringsEqual(value23, "cosmos");

    auto value24 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTheta));
    assertStringsEqual(value24, "theta");

    auto value25 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGroestlcoin));
    assertStringsEqual(value25, "groestlcoin");

    auto value26 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLux));
    assertStringsEqual(value26, "lux");

    auto value27 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeQtum));
    assertStringsEqual(value27, "qtum");

    auto value28 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthersocial));
    assertStringsEqual(value28, "ethersocial");
    auto value29 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIocoin));
    assertStringsEqual(value29, "iocoin");

    auto value30 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBravoCoin));
    assertStringsEqual(value30, "bravocoin");

    auto value31 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSteem));
    assertStringsEqual(value31, "steem");

    auto value32 = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEOS));
    assertStringsEqual(value32, "eos");
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

    auto value7 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGoChain));
    assertStringsEqual(value7, "GoChain");

    auto value8 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));
    assertStringsEqual(value8, "ICON");

    auto value9 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));
    assertStringsEqual(value9, "Litecoin");

    auto value10 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOANetwork));
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

    auto value18 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));
    assertStringsEqual(value18, "Tezos");

    auto value19 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeStellar));
    assertStringsEqual(value19, "Stellar");

    auto value20 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXDai));
    assertStringsEqual(value20, "xDai");

    auto value21 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAion));
    assertStringsEqual(value21, "Aion");

    auto value22 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));
    assertStringsEqual(value22, "XRP");

    auto value23 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKin));
    assertStringsEqual(value23, "Kin");

    auto value24 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCosmos));
    assertStringsEqual(value24, "Cosmos");

    auto value25 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTheta));
    assertStringsEqual(value25, "Theta");

    auto value26 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGroestlcoin));
    assertStringsEqual(value26, "Groestlcoin");


    auto value27 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLux));
    assertStringsEqual(value27, "Lux");

    auto value28 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeQtum));
    assertStringsEqual(value28, "Qtum");

    auto value29 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthersocial));
    assertStringsEqual(value29, "Ethersocial");
    auto value30 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIocoin));
    assertStringsEqual(value30, "Iocoin");

    auto value31 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBravoCoin));
    assertStringsEqual(value31, "BravoCoin");

    auto value32 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSteem));
    assertStringsEqual(value32, "Steem");

    auto value33 = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEOS));
    assertStringsEqual(value33, "EOS");
}
