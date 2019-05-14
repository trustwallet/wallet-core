// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <gtest/gtest.h>

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetSymbol) {
    auto eth = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereum));
    assertStringsEqual(eth, "ETH");

    auto btc = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoin));
    assertStringsEqual(btc, "BTC");

    auto bch = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinCash));
    assertStringsEqual(bch, "BCH");

    auto clo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    assertStringsEqual(clo, "CLO");

    auto dash = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDash));
    assertStringsEqual(dash, "DASH");

    auto decred = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDecred));
    assertStringsEqual(decred, "DCR");

    auto etc = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    assertStringsEqual(etc, "ETC");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "GO");

    auto icon = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    assertStringsEqual(icon, "ICX");

    auto ltc = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    assertStringsEqual(ltc, "LTC");

    auto poa = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "POA");

    auto thunder = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    assertStringsEqual(thunder, "TT");

    auto tomo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    assertStringsEqual(tomo, "TOMO");

    auto tron = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    assertStringsEqual(tron, "TRX");

    auto vet = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    assertStringsEqual(vet, "VET");

    auto wan = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    assertStringsEqual(wan, "XZC");

    auto xzc = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    assertStringsEqual(xzc, "BNB");

    auto bnb = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(bnb, "XTZ");

    auto eth9 = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    assertStringsEqual(eth9, "XLM");

    auto zec = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDai));
    assertStringsEqual(zec, "xDAI");

    auto xtz = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAion));
    assertStringsEqual(xtz, "AION");

    auto xlm = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    assertStringsEqual(xlm, "KIN");

    auto xdai = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    assertStringsEqual(xdai, "ATOM");

    auto aion = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    assertStringsEqual(aion, "THETA");

    auto kin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    assertStringsEqual(kin, "GRS");

    auto atom = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLux));
    assertStringsEqual(atom, "LUX");

    auto theta = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    assertStringsEqual(theta, "QTUM");

    auto grs = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthersocial));
    assertStringsEqual(grs, "ESN");

    auto doge = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIocoin));
    assertStringsEqual(doge, "IOC");

    auto lux = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    assertStringsEqual(lux, "BRAVO");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    assertStringsEqual(qtum, "STEEM");

    auto ets = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(ets, "EOS");

    auto iocoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    assertStringsEqual(iocoin, "NANO");

    auto bravo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIoTeX));
    assertStringsEqual(bravo, "IOTX");

    auto zil = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZilliqa));
    assertStringsEqual(zil, "ZIL");

    auto semux = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSemux));
    assertStringsEqual(semux, "SEM");

    auto ella = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEllaism));
    assertStringsEqual(ella, "ELLA");

    auto dexon = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDEXON));
    assertStringsEqual(dexon, "DXN");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetDecimals) {
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDecred), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEllaism), 18);
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
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNano), 30);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIoTeX), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZilliqa), 12);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSemux), 9);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDEXON), 18);
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetTransactionURL) {
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto eth = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereum, txId));
    assertStringsEqual(eth, "https://etherscan.io/tx/123");

    auto btc = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoin, txId));
    assertStringsEqual(btc, "https://blockchair.com/bitcoin/transaction/123");

    auto bch = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinCash, txId));
    assertStringsEqual(bch, "https://blockchair.com/bitcoin-cash/transaction/123");

    auto clo = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId));
    assertStringsEqual(clo, "https://explorer2.callisto.network/tx/123");

    auto dash = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDash, txId));
    assertStringsEqual(dash, "https://blockchair.com/dash/transaction/123");

    auto etc = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    assertStringsEqual(etc, "https://gastracker.io/tx/123");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGoChain, txId));
    assertStringsEqual(gochain, "https://explorer.gochain.io/tx/123");

    auto icon = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    assertStringsEqual(icon, "https://tracker.icon.foundation/transaction/123");

    auto ltc = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    assertStringsEqual(ltc, "https://blockchair.com/litecoin/transaction/123");

    auto poa = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOANetwork, txId));
    assertStringsEqual(poa, "https://poaexplorer.com/txid/search/123");

    auto thunder = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId));
    assertStringsEqual(thunder, "https://scan.thundercore.com/transactions/123");

    auto tomo = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId));
    assertStringsEqual(tomo, "https://scan.tomochain.com/txs/123");

    auto tron = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    assertStringsEqual(tron, "https://tronscan.org/#/transaction/123");

    auto vet = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId));
    assertStringsEqual(vet, "https://explore.veforge.com/transactions/123");

    auto wan = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId));
    assertStringsEqual(wan, "https://explorer.wanchain.org/tx/123");

    auto xzc = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    assertStringsEqual(xzc, "https://explorer.zcoin.io/tx/123");

    auto bnb = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    assertStringsEqual(bnb, "https://explorer.binance.org/tx/123");

    auto zecTxId = TWStringCreateWithUTF8Bytes("d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");
    auto zec = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, zecTxId));
    assertStringsEqual(zec, "https://chain.so/tx/ZEC/d831fda3a9e74d14cd151d035ab77cf0a71eea6c0e4aa0d5c1de54851c3c1d9e");

    auto xtz = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    assertStringsEqual(xtz, "https://tzscan.io/123");

    auto xlm = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeStellar, txId));
    assertStringsEqual(xlm, "https://stellarscan.io/transaction/123");

    auto xdai = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXDai, txId));
    assertStringsEqual(xdai, "https://blockscout.com/poa/dai/tx/123");

    auto aion = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAion, txId));
    assertStringsEqual(aion, "https://mainnet.aion.network/#/transaction/123");

    auto kin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKin, txId));
    assertStringsEqual(kin, "https://kinexplorer.com/tx/123");

    auto atom = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCosmos, txId));
    assertStringsEqual(atom, "https://www.mintscan.io/txs/123");

    auto theta = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTheta, txId));
    assertStringsEqual(theta, "https://explorer.thetatoken.org/txs/123");

    auto grs = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGroestlcoin, txId));
    assertStringsEqual(grs, "https://chainz.cryptoid.info/grs/tx.dws?123");

    auto doge = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDogecoin, txId));
    assertStringsEqual(doge, "https://live.blockcypher.com/doge/tx/123");

    auto lux = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLux, txId));
    assertStringsEqual(lux, "https://explorer.poswallet.io/#/tx/123");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId));
    assertStringsEqual(qtum, "https://qtum.info/tx/123");

    auto ets = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthersocial, txId));
    assertStringsEqual(ets, "https://ethersocial.net/tx/123");

    auto iocoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIocoin, txId));
    assertStringsEqual(iocoin, "https://chainz.cryptoid.info/ioc/tx.dws?123");

    auto bravo = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBravoCoin, txId));
    assertStringsEqual(bravo, "https://explorer.bravocoin.com/txid/123");

    auto steem = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSteem, txId));
    assertStringsEqual(steem, "https://steemblockexplorer.com/tx/123");

    auto eos = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEOS, txId));
    assertStringsEqual(eos, "https://bloks.io/transaction/123");

    auto nano = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNano, txId));
    assertStringsEqual(nano, "https://www.nanode.co/block/123");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIoTeX, txId));
    assertStringsEqual(iotex, "https://iotexscan.io/action/123");

    auto zil = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZilliqa, txId));
    assertStringsEqual(zil, "https://viewblock.io/zilliqa/tx/123");

    auto semux = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSemux, txId));
    assertStringsEqual(semux, "https://semux.info/explorer/transaction/123");

    auto ella = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEllaism, txId));
    assertStringsEqual(ella, "https://explorer.ellaism.org/tx/123");

    auto dexon = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDEXON, txId));
    assertStringsEqual(dexon, "https://dexonscan.app/transaction/123");

    auto zel = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZelcash, txId));
    assertStringsEqual(zel, "https://explorer.zel.cash/tx/123");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetID) {
    auto eth = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereum));
    assertStringsEqual(eth, "ethereum");

    auto btc = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoin));
    assertStringsEqual(btc, "bitcoin");

    auto bch = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinCash));
    assertStringsEqual(bch, "bitcoincash");

    auto clo = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    assertStringsEqual(clo, "callisto");

    auto dash = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDash));
    assertStringsEqual(dash, "dash");

    auto etc = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    assertStringsEqual(etc, "classic");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "gochain");

    auto icon = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    assertStringsEqual(icon, "icon");

    auto ltc = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    assertStringsEqual(ltc, "litecoin");

    auto poa = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "poa");

    auto thunder = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    assertStringsEqual(thunder, "thundertoken");

    auto tomo = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    assertStringsEqual(tomo, "tomochain");

    auto tron = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    assertStringsEqual(tron, "tron");

    auto vet = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    assertStringsEqual(vet, "vechain");

    auto wan = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWanchain));
    assertStringsEqual(wan, "wanchain");

    auto xzc = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    assertStringsEqual(xzc, "zcoin");

    auto bnb = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinance));
    assertStringsEqual(bnb, "binance");

    auto xtz = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    assertStringsEqual(xtz, "tezos");

    auto xlm = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeStellar));
    assertStringsEqual(xlm, "stellar");

    auto xdai = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXDai));
    assertStringsEqual(xdai, "xdai");

    auto aion = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAion));
    assertStringsEqual(aion, "aion");

    auto kin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKin));
    assertStringsEqual(kin, "kin");

    auto atom = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCosmos));
    assertStringsEqual(atom, "cosmos");

    auto theta = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTheta));
    assertStringsEqual(theta, "theta");

    auto grs = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGroestlcoin));
    assertStringsEqual(grs, "groestlcoin");

    auto lux = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLux));
    assertStringsEqual(lux, "lux");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeQtum));
    assertStringsEqual(qtum, "qtum");

    auto ets = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthersocial));
    assertStringsEqual(ets, "ethersocial");

    auto iocoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIocoin));
    assertStringsEqual(iocoin, "iocoin");

    auto bravo = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBravoCoin));
    assertStringsEqual(bravo, "bravocoin");

    auto steem = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSteem));
    assertStringsEqual(steem, "steem");

    auto eos = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEOS));
    assertStringsEqual(eos, "eos");

    auto nano = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNano));
    assertStringsEqual(nano, "nano");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "iotex");

    auto zil = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZilliqa));
    assertStringsEqual(zil, "zilliqa");

    auto semux = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSemux));
    assertStringsEqual(semux, "semux");

    auto ella = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEllaism));
    assertStringsEqual(ella, "ellaism");

    auto dexon = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDEXON));
    assertStringsEqual(dexon, "dexon");

    auto zel = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZelcash));
    assertStringsEqual(zel, "zelcash");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetName) {
    auto eth = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereum));
    assertStringsEqual(eth, "Ethereum");

    auto btc = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoin));
    assertStringsEqual(btc, "Bitcoin");

    auto bch = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinCash));
    assertStringsEqual(bch, "Bitcoin Cash");

    auto clo = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));
    assertStringsEqual(clo, "Callisto");

    auto dash = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDash));
    assertStringsEqual(dash, "Dash");

    auto etc = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));
    assertStringsEqual(etc, "Ethereum Classic");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "GoChain");

    auto icon = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));
    assertStringsEqual(icon, "ICON");

    auto ltc = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));
    assertStringsEqual(ltc, "Litecoin");

    auto poa = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "POA Network");

    auto thunder = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));
    assertStringsEqual(thunder, "Thunder Token");

    auto tomo = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));
    assertStringsEqual(tomo, "TomoChain");

    auto tron = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));
    assertStringsEqual(tron, "Tron");

    auto vet = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));
    assertStringsEqual(vet, "VeChain");

    auto wan = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWanchain));
    assertStringsEqual(wan, "Wanchain");

    auto xzc = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));
    assertStringsEqual(xzc, "Zcoin");

    auto bnb = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinance));
    assertStringsEqual(bnb, "Binance");

    auto xtz = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));
    assertStringsEqual(xtz, "Tezos");

    auto xlm = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeStellar));
    assertStringsEqual(xlm, "Stellar");

    auto xdai = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXDai));
    assertStringsEqual(xdai, "xDai");

    auto aion = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAion));
    assertStringsEqual(aion, "Aion");

    auto xrp = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));
    assertStringsEqual(xrp, "XRP");

    auto kin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKin));
    assertStringsEqual(kin, "Kin");

    auto atom = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCosmos));
    assertStringsEqual(atom, "Cosmos");

    auto theta = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTheta));
    assertStringsEqual(theta, "Theta");

    auto grs = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGroestlcoin));
    assertStringsEqual(grs, "Groestlcoin");

    auto lux = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLux));
    assertStringsEqual(lux, "Lux");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeQtum));
    assertStringsEqual(qtum, "Qtum");

    auto ets = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthersocial));
    assertStringsEqual(ets, "Ethersocial");

    auto iocoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIocoin));
    assertStringsEqual(iocoin, "Iocoin");

    auto bravo = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBravoCoin));
    assertStringsEqual(bravo, "BravoCoin");

    auto steem = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSteem));
    assertStringsEqual(steem, "Steem");

    auto eos = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEOS));
    assertStringsEqual(eos, "EOS");

    auto nano = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNano));
    assertStringsEqual(nano, "Nano");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "IoTeX");

    auto zil = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZilliqa));
    assertStringsEqual(zil, "Zilliqa");

    auto semux = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSemux));
    assertStringsEqual(semux, "Semux");

    auto ella = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEllaism));
    assertStringsEqual(ella, "Ellaism");

    auto dexon = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDEXON));
    assertStringsEqual(dexon, "DEXON");

    auto zel = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZelcash));
    assertStringsEqual(zel, "Zelcash");
}
