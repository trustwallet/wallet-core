// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <gtest/gtest.h>


TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetSymbol) {
    auto aeternity = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAeternity));
    assertStringsEqual(aeternity, "AE");

    auto aion = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAion));
    assertStringsEqual(aion, "AION");

    auto algorand = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAlgorand));
    assertStringsEqual(algorand, "ALGO");

    auto ark = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeARK));
    assertStringsEqual(ark, "ARK");

    auto binance = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    assertStringsEqual(binance, "BNB");

    auto bitcoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoin));
    assertStringsEqual(bitcoin, "BTC");

    auto bitcoincash = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinCash));
    assertStringsEqual(bitcoincash, "BCH");

    auto bravocoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    assertStringsEqual(bravocoin, "BRAVO");

    auto callisto = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    assertStringsEqual(callisto, "CLO");

    auto classic = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    assertStringsEqual(classic, "ETC");

    auto cosmos = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    assertStringsEqual(cosmos, "ATOM");

    auto dash = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDash));
    assertStringsEqual(dash, "DASH");

    auto decred = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDecred));
    assertStringsEqual(decred, "DCR");

    auto digibyte = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDigiByte));
    assertStringsEqual(digibyte, "DGB");

    auto doge = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDogecoin));
    assertStringsEqual(doge, "DOGE");

    auto eos = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(eos, "EOS");

    auto ethereum = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereum));
    assertStringsEqual(ethereum, "ETH");

    auto fio = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFIO));
    assertStringsEqual(fio, "FIO");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "GO");

    auto groestlcoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    assertStringsEqual(groestlcoin, "GRS");

    auto harmony = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeHarmony));
    assertStringsEqual(harmony, "ONE");

    auto icon = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    assertStringsEqual(icon, "ICX");

    auto iost = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIOST));
    assertStringsEqual(iost, "IOST");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "IOTX");

    auto kin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    assertStringsEqual(kin, "KIN");

    auto litecoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    assertStringsEqual(litecoin, "LTC");

    auto monacoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMonacoin));
    assertStringsEqual(monacoin, "MONA");

    auto nano = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    assertStringsEqual(nano, "NANO");

    auto near = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEAR));
    assertStringsEqual(near, "NEAR");

    auto nebulas = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNebulas));
    assertStringsEqual(nebulas, "NAS");

    auto neo = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEO));
    assertStringsEqual(neo, "NEO");

    auto nimiq = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNimiq));
    assertStringsEqual(nimiq, "NIM");

    auto nuls = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNULS));
    assertStringsEqual(nuls, "NULS");

    auto ontology = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOntology));
    assertStringsEqual(ontology, "ONT");

    auto poa = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "POA");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    assertStringsEqual(qtum, "QTUM");

    auto ravencoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRavencoin));
    assertStringsEqual(ravencoin, "RVN");

    auto ripple = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXRP));
    assertStringsEqual(ripple, "XRP");

    auto semux = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSemux));
    assertStringsEqual(semux, "SEM");

    auto solana = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSolana));
    assertStringsEqual(solana, "SOL");

    auto steem = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    assertStringsEqual(steem, "STEEM");

    auto stellar = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    assertStringsEqual(stellar, "XLM");

    auto terra = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTerra));
    assertStringsEqual(terra, "LUNA");

    auto tezos = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(tezos, "XTZ");

    auto theta = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    assertStringsEqual(theta, "THETA");

    auto thundertoken = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    assertStringsEqual(thundertoken, "TT");

    auto tomochain = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    assertStringsEqual(tomochain, "TOMO");

    auto tron = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    assertStringsEqual(tron, "TRX");

    auto vechain = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    assertStringsEqual(vechain, "VET");

    auto viacoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeViacoin));
    assertStringsEqual(viacoin, "VIA");

    auto wanchain = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWanchain));
    assertStringsEqual(wanchain, "WAN");

    auto waves = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWaves));
    assertStringsEqual(waves, "WAVES");

    auto zcash = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcash));
    assertStringsEqual(zcash, "ZEC");

    auto zcoin = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    assertStringsEqual(zcoin, "XZC");

    auto zelcash = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZelcash));
    assertStringsEqual(zelcash, "ZEL");

    auto zilliqa = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZilliqa));
    assertStringsEqual(zilliqa, "ZIL");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetDecimals) {
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAeternity), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAion), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAlgorand), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeARK), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinance), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBravoCoin), 3);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCosmos), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDecred), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDigiByte), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDogecoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEOS), 4);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFIO), 9);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGoChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGroestlcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeHarmony), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeICON), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIOST), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIoTeX), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKin), 5);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLitecoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMonacoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNano), 30);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEAR), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNebulas), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEO), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNimiq), 5);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNULS), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOntology), 0);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePOANetwork), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeQtum), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRavencoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXRP), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSemux), 9);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSolana), 13);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSteem), 3);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeStellar), 7);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTerra), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTheta), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeViacoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanchain), 18);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWaves), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZelcash), 8);
    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZilliqa), 12);
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetTransactionURL) {
    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto aeternity = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAeternity, txId));
    assertStringsEqual(aeternity, "https://explorer.aepps.com/#/tx/123");

    auto aion = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAion, txId));
    assertStringsEqual(aion, "https://mainnet.aion.network/#/transaction/123");

    auto algorand = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAlgorand, txId));
    assertStringsEqual(algorand, "https://algoexplorer.io/tx/123");

    auto ark = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeARK, txId));
    assertStringsEqual(ark, "https://explorer.ark.io/transaction/123");

    auto binance = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    assertStringsEqual(binance, "https://explorer.binance.org/tx/123");

    auto bitcoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoin, txId));
    assertStringsEqual(bitcoin, "https://blockchair.com/bitcoin/transaction/123");

    auto bitcoincash = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinCash, txId));
    assertStringsEqual(bitcoincash, "https://blockchair.com/bitcoin-cash/transaction/123");

    auto bravocoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBravoCoin, txId));
    assertStringsEqual(bravocoin, "https://explorer.bravocoin.com/txid/123");

    auto callisto = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId));
    assertStringsEqual(callisto, "https://explorer2.callisto.network/tx/123");

    auto classic = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    assertStringsEqual(classic, "https://gastracker.io/tx/123");

    auto cosmos = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCosmos, txId));
    assertStringsEqual(cosmos, "https://www.mintscan.io/txs/123");

    auto dash = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDash, txId));
    assertStringsEqual(dash, "https://blockchair.com/dash/transaction/123");

    auto decred = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDecred, txId));
    assertStringsEqual(decred, "https://dcrdata.decred.org/tx/123");

    auto digibyte = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDigiByte, txId));
    assertStringsEqual(digibyte, "https://digiexplorer.info/tx/123");

    auto doge = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDogecoin, txId));
    assertStringsEqual(doge, "https://blockchair.com/dogecoin/transaction/123");

    auto eos = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEOS, txId));
    assertStringsEqual(eos, "https://bloks.io/transaction/123");

    auto ethereum = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereum, txId));
    assertStringsEqual(ethereum, "https://etherscan.io/tx/123");

    auto fio = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFIO, txId));
    assertStringsEqual(fio, "https://fio.foundation123");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGoChain, txId));
    assertStringsEqual(gochain, "https://explorer.gochain.io/tx/123");

    auto groestlcoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGroestlcoin, txId));
    assertStringsEqual(groestlcoin, "https://blockchair.com/groestlcoin/transaction/123");

    auto harmony = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeHarmony, txId));
    assertStringsEqual(harmony, "https://explorer.harmony.one/#/tx/123");

    auto icon = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    assertStringsEqual(icon, "https://tracker.icon.foundation/transaction/123");

    auto iost = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIOST, txId));
    assertStringsEqual(iost, "https://www.iostabc.com/tx/123");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIoTeX, txId));
    assertStringsEqual(iotex, "https://iotexscan.io/action/123");

    auto kin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKin, txId));
    assertStringsEqual(kin, "https://kinexplorer.com/tx/123");

    auto litecoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    assertStringsEqual(litecoin, "https://blockchair.com/litecoin/transaction/123");

    auto monacoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMonacoin, txId));
    assertStringsEqual(monacoin, "https://blockbook.electrum-mona.org/tx/123");

    auto nano = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNano, txId));
    assertStringsEqual(nano, "https://www.nanode.co/block/123");

    auto near = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEAR, txId));
    assertStringsEqual(near, "https://explorer.nearprotocol.com123");

    auto nebulas = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNebulas, txId));
    assertStringsEqual(nebulas, "https://explorer.nebulas.io/#/tx/123");

    auto neo = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEO, txId));
    assertStringsEqual(neo, "https://neoscan.io/transaction/123");

    auto nimiq = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNimiq, txId));
    assertStringsEqual(nimiq, "https://nimiq.watch/#123");

    auto nuls = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNULS, txId));
    assertStringsEqual(nuls, "https://nulscan.io/123");

    auto ontology = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOntology, txId));
    assertStringsEqual(ontology, "https://explorer.ont.io/transaction/123");

    auto poa = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOANetwork, txId));
    assertStringsEqual(poa, "https://poaexplorer.com/txid/search/123");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId));
    assertStringsEqual(qtum, "https://qtum.info/tx/123");

    auto ravencoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRavencoin, txId));
    assertStringsEqual(ravencoin, "https://ravencoin.network/tx/123");

    auto ripple = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXRP, txId));
    assertStringsEqual(ripple, "https://bithomp.com/explorer/123");

    auto semux = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSemux, txId));
    assertStringsEqual(semux, "https://semux.info/explorer/transaction/123");

    auto solana = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSolana, txId));
    assertStringsEqual(solana, "https://explorer.solana.com/tx/123");

    auto steem = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSteem, txId));
    assertStringsEqual(steem, "https://steemblockexplorer.com/tx/123");

    auto stellar = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeStellar, txId));
    assertStringsEqual(stellar, "https://stellarscan.io/transaction/123");

    auto terra = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTerra, txId));
    assertStringsEqual(terra, "https://terra.stake.id/?#/tx/123");

    auto tezos = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    assertStringsEqual(tezos, "https://tezos.id/123");

    auto theta = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTheta, txId));
    assertStringsEqual(theta, "https://explorer.thetatoken.org/txs/123");

    auto thundertoken = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId));
    assertStringsEqual(thundertoken, "https://scan.thundercore.com/transactions/123");

    auto tomochain = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId));
    assertStringsEqual(tomochain, "https://scan.tomochain.com/txs/123");

    auto tron = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    assertStringsEqual(tron, "https://tronscan.org/#/transaction/123");

    auto vechain = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId));
    assertStringsEqual(vechain, "https://insight.vecha.in/#/txs/123");

    auto viacoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeViacoin, txId));
    assertStringsEqual(viacoin, "https://explorer.viacoin.org/tx/123");

    auto wanchain = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId));
    assertStringsEqual(wanchain, "https://explorer.wanchain.org/tx/123");

    auto waves = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWaves, txId));
    assertStringsEqual(waves, "https://wavesexplorer.com/tx/123");

    auto zcash = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, txId));
    assertStringsEqual(zcash, "https://chain.so/tx/ZEC/123");

    auto zcoin = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    assertStringsEqual(zcoin, "https://explorer.zcoin.io/tx/123");

    auto zelcash = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZelcash, txId));
    assertStringsEqual(zelcash, "https://explorer.zel.cash/tx/123");

    auto zilliqa = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZilliqa, txId));
    assertStringsEqual(zilliqa, "https://viewblock.io/zilliqa/tx/123");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetID) {
    auto aeternity = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAeternity));
    assertStringsEqual(aeternity, "aeternity");

    auto aion = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAion));
    assertStringsEqual(aion, "aion");

    auto algorand = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAlgorand));
    assertStringsEqual(algorand, "algorand");

    auto ark = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeARK));
    assertStringsEqual(ark, "ark");

    auto binance = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinance));
    assertStringsEqual(binance, "binance");

    auto bitcoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoin));
    assertStringsEqual(bitcoin, "bitcoin");

    auto bitcoincash = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinCash));
    assertStringsEqual(bitcoincash, "bitcoincash");

    auto bravocoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBravoCoin));
    assertStringsEqual(bravocoin, "bravocoin");

    auto callisto = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    assertStringsEqual(callisto, "callisto");

    auto classic = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    assertStringsEqual(classic, "classic");

    auto cosmos = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCosmos));
    assertStringsEqual(cosmos, "cosmos");

    auto dash = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDash));
    assertStringsEqual(dash, "dash");

    auto decred = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDecred));
    assertStringsEqual(decred, "decred");

    auto digibyte = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDigiByte));
    assertStringsEqual(digibyte, "digibyte");

    auto doge = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDogecoin));
    assertStringsEqual(doge, "doge");

    auto eos = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEOS));
    assertStringsEqual(eos, "eos");

    auto ethereum = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereum));
    assertStringsEqual(ethereum, "ethereum");

    auto fio = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFIO));
    assertStringsEqual(fio, "fio");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "gochain");

    auto groestlcoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGroestlcoin));
    assertStringsEqual(groestlcoin, "groestlcoin");

    auto harmony = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeHarmony));
    assertStringsEqual(harmony, "harmony");

    auto icon = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    assertStringsEqual(icon, "icon");

    auto iost = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIOST));
    assertStringsEqual(iost, "iost");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "iotex");

    auto kin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKin));
    assertStringsEqual(kin, "kin");

    auto litecoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    assertStringsEqual(litecoin, "litecoin");

    auto monacoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMonacoin));
    assertStringsEqual(monacoin, "monacoin");

    auto nano = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNano));
    assertStringsEqual(nano, "nano");

    auto near = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEAR));
    assertStringsEqual(near, "near");

    auto nebulas = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNebulas));
    assertStringsEqual(nebulas, "nebulas");

    auto neo = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEO));
    assertStringsEqual(neo, "neo");

    auto nimiq = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNimiq));
    assertStringsEqual(nimiq, "nimiq");

    auto nuls = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNULS));
    assertStringsEqual(nuls, "nuls");

    auto ontology = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOntology));
    assertStringsEqual(ontology, "ontology");

    auto poa = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "poa");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeQtum));
    assertStringsEqual(qtum, "qtum");

    auto ravencoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRavencoin));
    assertStringsEqual(ravencoin, "ravencoin");

    auto ripple = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXRP));
    assertStringsEqual(ripple, "ripple");

    auto semux = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSemux));
    assertStringsEqual(semux, "semux");

    auto solana = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSolana));
    assertStringsEqual(solana, "solana");

    auto steem = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSteem));
    assertStringsEqual(steem, "steem");

    auto stellar = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeStellar));
    assertStringsEqual(stellar, "stellar");

    auto terra = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTerra));
    assertStringsEqual(terra, "terra");

    auto tezos = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    assertStringsEqual(tezos, "tezos");

    auto theta = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTheta));
    assertStringsEqual(theta, "theta");

    auto thundertoken = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    assertStringsEqual(thundertoken, "thundertoken");

    auto tomochain = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    assertStringsEqual(tomochain, "tomochain");

    auto tron = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    assertStringsEqual(tron, "tron");

    auto vechain = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    assertStringsEqual(vechain, "vechain");

    auto viacoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeViacoin));
    assertStringsEqual(viacoin, "viacoin");

    auto wanchain = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWanchain));
    assertStringsEqual(wanchain, "wanchain");

    auto waves = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWaves));
    assertStringsEqual(waves, "waves");

    auto zcash = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcash));
    assertStringsEqual(zcash, "zcash");

    auto zcoin = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    assertStringsEqual(zcoin, "zcoin");

    auto zelcash = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZelcash));
    assertStringsEqual(zelcash, "zelcash");

    auto zilliqa = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZilliqa));
    assertStringsEqual(zilliqa, "zilliqa");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeConfigurationGetName) {
    auto aeternity = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAeternity));
    assertStringsEqual(aeternity, "Aeternity");

    auto aion = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAion));
    assertStringsEqual(aion, "Aion");

    auto algorand = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAlgorand));
    assertStringsEqual(algorand, "Algorand");

    auto ark = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeARK));
    assertStringsEqual(ark, "ARK");

    auto binance = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinance));
    assertStringsEqual(binance, "BNB");

    auto bitcoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoin));
    assertStringsEqual(bitcoin, "Bitcoin");

    auto bitcoincash = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinCash));
    assertStringsEqual(bitcoincash, "Bitcoin Cash");

    auto bravocoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBravoCoin));
    assertStringsEqual(bravocoin, "BravoCoin");

    auto callisto = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));
    assertStringsEqual(callisto, "Callisto");

    auto classic = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));
    assertStringsEqual(classic, "Ethereum Classic");

    auto cosmos = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCosmos));
    assertStringsEqual(cosmos, "Cosmos");

    auto dash = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDash));
    assertStringsEqual(dash, "Dash");

    auto decred = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDecred));
    assertStringsEqual(decred, "Decred");

    auto digibyte = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDigiByte));
    assertStringsEqual(digibyte, "DigiByte");

    auto doge = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDogecoin));
    assertStringsEqual(doge, "Dogecoin");

    auto eos = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEOS));
    assertStringsEqual(eos, "EOS");

    auto ethereum = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereum));
    assertStringsEqual(ethereum, "Ethereum");

    auto fio = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFIO));
    assertStringsEqual(fio, "FIO");

    auto gochain = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGoChain));
    assertStringsEqual(gochain, "GoChain");

    auto groestlcoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGroestlcoin));
    assertStringsEqual(groestlcoin, "Groestlcoin");

    auto harmony = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeHarmony));
    assertStringsEqual(harmony, "Harmony");

    auto icon = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));
    assertStringsEqual(icon, "ICON");

    auto iost = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIOST));
    assertStringsEqual(iost, "IOST");

    auto iotex = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIoTeX));
    assertStringsEqual(iotex, "IoTeX");

    auto kin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKin));
    assertStringsEqual(kin, "Kin");

    auto litecoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));
    assertStringsEqual(litecoin, "Litecoin");

    auto monacoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMonacoin));
    assertStringsEqual(monacoin, "Monacoin");

    auto nano = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNano));
    assertStringsEqual(nano, "Nano");

    auto near = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEAR));
    assertStringsEqual(near, "NEAR");

    auto nebulas = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNebulas));
    assertStringsEqual(nebulas, "Nebulas");

    auto neo = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEO));
    assertStringsEqual(neo, "NEO");

    auto nimiq = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNimiq));
    assertStringsEqual(nimiq, "Nimiq");

    auto nuls = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNULS));
    assertStringsEqual(nuls, "NULS");

    auto ontology = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOntology));
    assertStringsEqual(ontology, "Ontology");

    auto poa = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOANetwork));
    assertStringsEqual(poa, "POA Network");

    auto qtum = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeQtum));
    assertStringsEqual(qtum, "Qtum");

    auto ravencoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRavencoin));
    assertStringsEqual(ravencoin, "Ravencoin");

    auto ripple = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));
    assertStringsEqual(ripple, "XRP");

    auto semux = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSemux));
    assertStringsEqual(semux, "Semux");

    auto solana = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSolana));
    assertStringsEqual(solana, "Solana");

    auto steem = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSteem));
    assertStringsEqual(steem, "Steem");

    auto stellar = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeStellar));
    assertStringsEqual(stellar, "Stellar");

    auto terra = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTerra));
    assertStringsEqual(terra, "Terra");

    auto tezos = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));
    assertStringsEqual(tezos, "Tezos");

    auto theta = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTheta));
    assertStringsEqual(theta, "Theta");

    auto thundertoken = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));
    assertStringsEqual(thundertoken, "Thunder Token");

    auto tomochain = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));
    assertStringsEqual(tomochain, "TomoChain");

    auto tron = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));
    assertStringsEqual(tron, "Tron");

    auto vechain = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));
    assertStringsEqual(vechain, "VeChain");

    auto viacoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeViacoin));
    assertStringsEqual(viacoin, "Viacoin");

    auto wanchain = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWanchain));
    assertStringsEqual(wanchain, "Wanchain");

    auto waves = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWaves));
    assertStringsEqual(waves, "Waves");

    auto zcash = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcash));
    assertStringsEqual(zcash, "Zcash");

    auto zcoin = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));
    assertStringsEqual(zcoin, "Zcoin");

    auto zelcash = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZelcash));
    assertStringsEqual(zelcash, "Zelcash");

    auto zilliqa = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZilliqa));
    assertStringsEqual(zilliqa, "Zilliqa");
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBlockchain) {
    ASSERT_EQ(TWBlockchainAeternity, TWCoinTypeBlockchain(TWCoinTypeAeternity));
    ASSERT_EQ(TWBlockchainAion, TWCoinTypeBlockchain(TWCoinTypeAion));
    ASSERT_EQ(TWBlockchainAlgorand, TWCoinTypeBlockchain(TWCoinTypeAlgorand));
    ASSERT_EQ(TWBlockchainArk, TWCoinTypeBlockchain(TWCoinTypeARK));
    ASSERT_EQ(TWBlockchainBinance, TWCoinTypeBlockchain(TWCoinTypeBinance));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoin));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoinCash));
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeBravoCoin));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCallisto));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereumClassic));
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCosmos));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDash));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDecred));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDigiByte));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDogecoin));
    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeEOS));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereum));
    ASSERT_EQ(TWBlockchainFIO, TWCoinTypeBlockchain(TWCoinTypeFIO));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeGoChain));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeGroestlcoin));
    ASSERT_EQ(TWBlockchainHarmony, TWCoinTypeBlockchain(TWCoinTypeHarmony));
    ASSERT_EQ(TWBlockchainIcon, TWCoinTypeBlockchain(TWCoinTypeICON));
    ASSERT_EQ(TWBlockchainIOST, TWCoinTypeBlockchain(TWCoinTypeIOST));
    ASSERT_EQ(TWBlockchainIoTeX, TWCoinTypeBlockchain(TWCoinTypeIoTeX));
    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeKin));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeLitecoin));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeMonacoin));
    ASSERT_EQ(TWBlockchainNano, TWCoinTypeBlockchain(TWCoinTypeNano));
    ASSERT_EQ(TWBlockchainNEAR, TWCoinTypeBlockchain(TWCoinTypeNEAR));
    ASSERT_EQ(TWBlockchainNebulas, TWCoinTypeBlockchain(TWCoinTypeNebulas));
    ASSERT_EQ(TWBlockchainNEO, TWCoinTypeBlockchain(TWCoinTypeNEO));
    ASSERT_EQ(TWBlockchainNimiq, TWCoinTypeBlockchain(TWCoinTypeNimiq));
    ASSERT_EQ(TWBlockchainNULS, TWCoinTypeBlockchain(TWCoinTypeNULS));
    ASSERT_EQ(TWBlockchainOntology, TWCoinTypeBlockchain(TWCoinTypeOntology));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePOANetwork));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeQtum));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeRavencoin));
    ASSERT_EQ(TWBlockchainRipple, TWCoinTypeBlockchain(TWCoinTypeXRP));
    ASSERT_EQ(TWBlockchainSemux, TWCoinTypeBlockchain(TWCoinTypeSemux));
    ASSERT_EQ(TWBlockchainSolana, TWCoinTypeBlockchain(TWCoinTypeSolana));
    ASSERT_EQ(TWBlockchainSteem, TWCoinTypeBlockchain(TWCoinTypeSteem));
    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeStellar));
    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeTerra));
    ASSERT_EQ(TWBlockchainTezos, TWCoinTypeBlockchain(TWCoinTypeTezos));
    ASSERT_EQ(TWBlockchainTheta, TWCoinTypeBlockchain(TWCoinTypeTheta));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeThunderToken));
    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeTomoChain));
    ASSERT_EQ(TWBlockchainTron, TWCoinTypeBlockchain(TWCoinTypeTron));
    ASSERT_EQ(TWBlockchainVechain, TWCoinTypeBlockchain(TWCoinTypeVeChain));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeViacoin));
    ASSERT_EQ(TWBlockchainWanchain, TWCoinTypeBlockchain(TWCoinTypeWanchain));
    ASSERT_EQ(TWBlockchainWaves, TWCoinTypeBlockchain(TWCoinTypeWaves));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcash));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcoin));
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZelcash));
    ASSERT_EQ(TWBlockchainZilliqa, TWCoinTypeBlockchain(TWCoinTypeZilliqa));
}

TEST(TWCoinTypeConfiguration, P2SHPrefix) {
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinCash));
    ASSERT_EQ(0x10, TWCoinTypeP2shPrefix(TWCoinTypeDash));
    ASSERT_EQ(0x1a, TWCoinTypeP2shPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x3f, TWCoinTypeP2shPrefix(TWCoinTypeDigiByte));
    ASSERT_EQ(0x16, TWCoinTypeP2shPrefix(TWCoinTypeDogecoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeGroestlcoin));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeLitecoin));
    ASSERT_EQ(0x37, TWCoinTypeP2shPrefix(TWCoinTypeMonacoin));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeQtum));
    ASSERT_EQ(0x7a, TWCoinTypeP2shPrefix(TWCoinTypeRavencoin));
    ASSERT_EQ(0x21, TWCoinTypeP2shPrefix(TWCoinTypeViacoin));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x07, TWCoinTypeP2shPrefix(TWCoinTypeZcoin));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZelcash));
}

TEST(TWCoinTypeConfiguration, StaticPrefix) {
    ASSERT_EQ(0x07, TWCoinTypeStaticPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZelcash));
}
