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

    auto zcoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    assertStringsEqual(zcoin, "XZC");

    auto bnb = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    assertStringsEqual(bnb, "BNB");

    auto xtz = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(xtz, "XTZ");

    auto xlm = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    assertStringsEqual(xlm, "XLM");

    auto xdai = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXDai));
    assertStringsEqual(xdai, "xDAI");

    auto aion = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAion));
    assertStringsEqual(aion, "AION");

    auto kin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    assertStringsEqual(kin, "KIN");

    auto atom = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    assertStringsEqual(atom, "ATOM");

    auto theta = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    assertStringsEqual(theta, "THETA");

    auto grs = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    assertStringsEqual(grs, "GRS");

    auto lux = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLux));
    assertStringsEqual(lux, "LUX");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    assertStringsEqual(qtum, "QTUM");

    auto esn = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthersocial));
    assertStringsEqual(esn, "ESN");

    auto bravo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    assertStringsEqual(bravo, "BRAVO");

    auto steem = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    assertStringsEqual(steem, "STEEM");

    auto eos = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(eos, "EOS");

    auto nano = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    assertStringsEqual(nano, "NANO");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "IOTX");

    auto zil = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZilliqa));
    assertStringsEqual(zil, "ZIL");

    auto semux = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSemux));
    assertStringsEqual(semux, "SEM");

    auto ella = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEllaism));
    assertStringsEqual(ella, "ELLA");

    auto dexon = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDEXON));
    assertStringsEqual(dexon, "DXN");

    auto dgb = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDigiByte));
    assertStringsEqual(dgb, "DGB");

    auto ark = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeARK));
    assertStringsEqual(ark, "ARK");

    auto waves = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWaves));
    assertStringsEqual(waves, "WAVES");

    auto nas = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNebulas));
    assertStringsEqual(nas, "NAS");

    auto ae = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAeternity));
    assertStringsEqual(ae, "AE");

    auto terra = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTerra));
    assertStringsEqual(terra, "LUNA");

    auto mona = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMonacoin));
    assertStringsEqual(mona, "MONA");

    auto fio = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFIO));
    assertStringsEqual(fio, "FIO");

    auto hmy = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeHarmony));
    assertStringsEqual(hmy, "ONE");

    auto solana = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSolana));
    assertStringsEqual(solana, "SOL");

    auto algo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAlgorand));
    assertStringsEqual(algo, "ALGO");
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
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDigiByte), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeARK), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWaves), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNebulas), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAeternity), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTerra), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMonacoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFIO), 9);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeHarmony), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSolana), 13);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAlgorand), 6);
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

    auto zec = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, txId));
    assertStringsEqual(zec, "https://chain.so/tx/ZEC/123");

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
    assertStringsEqual(grs, "https://blockchair.com/groestlcoin/transaction/123");

    auto doge = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDogecoin, txId));
    assertStringsEqual(doge, "https://blockchair.com/dogecoin/transaction/123");

    auto decred = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDecred, txId));
    assertStringsEqual(decred, "https://dcrdata.decred.org/tx/123");

    auto lux = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLux, txId));
    assertStringsEqual(lux, "https://explorer.poswallet.io/#/tx/123");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId));
    assertStringsEqual(qtum, "https://qtum.info/tx/123");

    auto ets = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthersocial, txId));
    assertStringsEqual(ets, "https://ethersocial.net/tx/123");

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

    auto dgb = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDigiByte, txId));
    assertStringsEqual(dgb, "https://digiexplorer.info/tx/123");

    auto ark = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeARK, txId));
    assertStringsEqual(ark, "https://explorer.ark.io/transaction/123");

    auto waves = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWaves, txId));
    assertStringsEqual(waves, "https://wavesexplorer.com/tx/123");

    auto nas = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNebulas, txId));
    assertStringsEqual(nas, "https://explorer.nebulas.io/#/tx/123");

    auto ae = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAeternity, txId));
    assertStringsEqual(ae, "https://explorer.aepps.com/#/tx/123");

    auto terra = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTerra, txId));
    assertStringsEqual(terra, "https://terra.stake.id/?#/tx/123");

    auto mona = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMonacoin, txId));
    assertStringsEqual(mona, "https://blockbook.electrum-mona.org/tx/123");

    auto harmony = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeHarmony, txId));
    assertStringsEqual(harmony, "https://explorer.harmony.one/#/tx/123");

    auto solana = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSolana, txId));
    assertStringsEqual(solana, "https://explorer.solana.com/tx/123");

    auto algo = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAlgorand, txId));
    assertStringsEqual(algo, "https://algoexplorer.io/tx/123");
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

    auto dgb = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDigiByte));
    assertStringsEqual(dgb, "digibyte");

    auto ark = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeARK));
    assertStringsEqual(ark, "ark");

    auto waves = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWaves));
    assertStringsEqual(waves, "waves");

    auto nas = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNebulas));
    assertStringsEqual(nas, "nebulas");

    auto ae = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAeternity));
    assertStringsEqual(ae, "aeternity");

    auto terra = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTerra));
    assertStringsEqual(terra, "terra");

    auto mona = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMonacoin));
    assertStringsEqual(mona, "monacoin");

    auto fio = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFIO));
    assertStringsEqual(fio, "fio");

    auto hmy = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeHarmony));
    assertStringsEqual(hmy, "harmony");

    auto solana = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSolana));
    assertStringsEqual(solana, "solana");

    auto algo = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAlgorand));
    assertStringsEqual(algo, "algorand");
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

    auto digibyte = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDigiByte));
    assertStringsEqual(digibyte, "DigiByte");

    auto ark = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeARK));
    assertStringsEqual(ark, "ARK");

    auto waves = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWaves));
    assertStringsEqual(waves, "Waves");

    auto nas = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNebulas));
    assertStringsEqual(nas, "Nebulas");

    auto terra = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTerra));
    assertStringsEqual(terra, "Terra");

    auto mona = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMonacoin));
    assertStringsEqual(mona, "Monacoin");

    auto fio = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFIO));
    assertStringsEqual(fio, "FIO");

    auto hmy = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeHarmony));
    assertStringsEqual(hmy, "Harmony");

    auto solana = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSolana));
    assertStringsEqual(solana, "Solana");

    auto algo = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAlgorand));
    assertStringsEqual(algo, "Algorand");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBlockchain) {
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoin));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeLitecoin));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereum));
    ASSERT_EQ(TWBlockchainIoTeX, TWCoinTypeBlockchain(TWCoinTypeIoTeX));
    ASSERT_EQ(TWBlockchainArk, TWCoinTypeBlockchain(TWCoinTypeARK));
    ASSERT_EQ(TWBlockchainOntology, TWCoinTypeBlockchain(TWCoinTypeOntology));
    ASSERT_EQ(TWBlockchainNEO, TWCoinTypeBlockchain(TWCoinTypeNEO));
    ASSERT_EQ(TWBlockchainIOST, TWCoinTypeBlockchain(TWCoinTypeIOST));
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeEOS));
    ASSERT_EQ(TWBlockchainSteem, TWCoinTypeBlockchain(TWCoinTypeSteem));
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeBravoCoin));
    ASSERT_EQ(TWBlockchainNano, TWCoinTypeBlockchain(TWCoinTypeNano));
    ASSERT_EQ(TWBlockchainSemux, TWCoinTypeBlockchain(TWCoinTypeSemux));
    ASSERT_EQ(TWBlockchainZilliqa, TWCoinTypeBlockchain(TWCoinTypeZilliqa));
    ASSERT_EQ(TWBlockchainWaves, TWCoinTypeBlockchain(TWCoinTypeWaves));
    ASSERT_EQ(TWBlockchainNebulas, TWCoinTypeBlockchain(TWCoinTypeNebulas));
    ASSERT_EQ(TWBlockchainAeternity, TWCoinTypeBlockchain(TWCoinTypeAeternity));
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeTerra));
    ASSERT_EQ(TWBlockchainFIO, TWCoinTypeBlockchain(TWCoinTypeFIO));
    ASSERT_EQ(TWBlockchainHarmony, TWCoinTypeBlockchain(TWCoinTypeHarmony));
    ASSERT_EQ(TWBlockchainSolana, TWCoinTypeBlockchain(TWCoinTypeSolana));
    ASSERT_EQ(TWBlockchainAlgorand, TWCoinTypeBlockchain(TWCoinTypeAlgorand));
}

TEST(TWCoinTypeConfiguration, P2SHPrefix) {
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinCash));
    ASSERT_EQ(0x10, TWCoinTypeP2shPrefix(TWCoinTypeDash));
    ASSERT_EQ(0x1a, TWCoinTypeP2shPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x16, TWCoinTypeP2shPrefix(TWCoinTypeDogecoin));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeLitecoin));
    ASSERT_EQ(0x21, TWCoinTypeP2shPrefix(TWCoinTypeViacoin));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x07, TWCoinTypeP2shPrefix(TWCoinTypeZcoin));
    ASSERT_EQ(0x3F, TWCoinTypeP2shPrefix(TWCoinTypeDigiByte));
    ASSERT_EQ(0x7a, TWCoinTypeP2shPrefix(TWCoinTypeRavencoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeGroestlcoin));
    ASSERT_EQ(0x37, TWCoinTypeP2shPrefix(TWCoinTypeMonacoin));
}

TEST(TWCoinTypeConfiguration, StaticPrefix) {
    ASSERT_EQ(0, TWCoinTypeStaticPrefix(TWCoinTypeBitcoin));
    ASSERT_EQ(0x07, TWCoinTypeStaticPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZcash));
}
