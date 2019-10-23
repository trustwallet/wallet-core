// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <gtest/gtest.h>


TEST(TWCoinTypeConfiguration, TWCoinTypeAeternity) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAeternity));
    assertStringsEqual(symbol, "AE");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAeternity), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAeternity, txId));
    assertStringsEqual(txUrl, "https://explorer.aepps.com/#/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAeternity));
    assertStringsEqual(id, "aeternity");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAeternity));
    assertStringsEqual(name, "Aeternity");

    ASSERT_EQ(TWBlockchainAeternity, TWCoinTypeBlockchain(TWCoinTypeAeternity));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeAion) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAion));
    assertStringsEqual(symbol, "AION");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAion), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAion, txId));
    assertStringsEqual(txUrl, "https://mainnet.aion.network/#/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAion));
    assertStringsEqual(id, "aion");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAion));
    assertStringsEqual(name, "Aion");

    ASSERT_EQ(TWBlockchainAion, TWCoinTypeBlockchain(TWCoinTypeAion));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeAlgorand) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeAlgorand));
    assertStringsEqual(symbol, "ALGO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeAlgorand), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeAlgorand, txId));
    assertStringsEqual(txUrl, "https://algoexplorer.io/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeAlgorand));
    assertStringsEqual(id, "algorand");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeAlgorand));
    assertStringsEqual(name, "Algorand");

    ASSERT_EQ(TWBlockchainAlgorand, TWCoinTypeBlockchain(TWCoinTypeAlgorand));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeARK) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeARK));
    assertStringsEqual(symbol, "ARK");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeARK), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeARK, txId));
    assertStringsEqual(txUrl, "https://explorer.ark.io/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeARK));
    assertStringsEqual(id, "ark");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeARK));
    assertStringsEqual(name, "ARK");

    ASSERT_EQ(TWBlockchainArk, TWCoinTypeBlockchain(TWCoinTypeARK));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBinance) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBinance));
    assertStringsEqual(symbol, "BNB");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBinance), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBinance, txId));
    assertStringsEqual(txUrl, "https://explorer.binance.org/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBinance));
    assertStringsEqual(id, "binance");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBinance));
    assertStringsEqual(name, "BNB");

    ASSERT_EQ(TWBlockchainBinance, TWCoinTypeBlockchain(TWCoinTypeBinance));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBitcoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoin));
    assertStringsEqual(symbol, "BTC");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoin, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/bitcoin/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoin));
    assertStringsEqual(id, "bitcoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoin));
    assertStringsEqual(name, "Bitcoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBitcoinCash) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBitcoinCash));
    assertStringsEqual(symbol, "BCH");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBitcoinCash), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBitcoinCash, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/bitcoin-cash/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBitcoinCash));
    assertStringsEqual(id, "bitcoincash");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBitcoinCash));
    assertStringsEqual(name, "Bitcoin Cash");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeBitcoinCash));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeBitcoinCash));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeBravoCoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeBravoCoin));
    assertStringsEqual(symbol, "BRAVO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeBravoCoin), 3);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeBravoCoin, txId));
    assertStringsEqual(txUrl, "https://explorer.bravocoin.com/txid/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeBravoCoin));
    assertStringsEqual(id, "bravocoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeBravoCoin));
    assertStringsEqual(name, "BravoCoin");

    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeBravoCoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeCallisto) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCallisto));
    assertStringsEqual(symbol, "CLO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCallisto), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCallisto, txId));
    assertStringsEqual(txUrl, "https://explorer2.callisto.network/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCallisto));
    assertStringsEqual(id, "callisto");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCallisto));
    assertStringsEqual(name, "Callisto");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeCallisto));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeEthereumClassic) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereumClassic));
    assertStringsEqual(symbol, "ETC");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereumClassic), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereumClassic, txId));
    assertStringsEqual(txUrl, "https://gastracker.io/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereumClassic));
    assertStringsEqual(id, "classic");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereumClassic));
    assertStringsEqual(name, "Ethereum Classic");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereumClassic));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeCosmos) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeCosmos));
    assertStringsEqual(symbol, "ATOM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeCosmos), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeCosmos, txId));
    assertStringsEqual(txUrl, "https://www.mintscan.io/txs/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeCosmos));
    assertStringsEqual(id, "cosmos");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeCosmos));
    assertStringsEqual(name, "Cosmos");

    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeCosmos));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeDash) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDash));
    assertStringsEqual(symbol, "DASH");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDash), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDash, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/dash/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDash));
    assertStringsEqual(id, "dash");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDash));
    assertStringsEqual(name, "Dash");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDash));
    ASSERT_EQ(0x10, TWCoinTypeP2shPrefix(TWCoinTypeDash));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeDecred) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDecred));
    assertStringsEqual(symbol, "DCR");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDecred), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDecred, txId));
    assertStringsEqual(txUrl, "https://dcrdata.decred.org/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDecred));
    assertStringsEqual(id, "decred");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDecred));
    assertStringsEqual(name, "Decred");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDecred));
    ASSERT_EQ(0x1a, TWCoinTypeP2shPrefix(TWCoinTypeDecred));
    ASSERT_EQ(0x07, TWCoinTypeStaticPrefix(TWCoinTypeDecred));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeDigiByte) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDigiByte));
    assertStringsEqual(symbol, "DGB");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDigiByte), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDigiByte, txId));
    assertStringsEqual(txUrl, "https://digiexplorer.info/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDigiByte));
    assertStringsEqual(id, "digibyte");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDigiByte));
    assertStringsEqual(name, "DigiByte");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDigiByte));
    ASSERT_EQ(0x3f, TWCoinTypeP2shPrefix(TWCoinTypeDigiByte));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeDogecoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDogecoin));
    assertStringsEqual(symbol, "DOGE");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDogecoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDogecoin, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/dogecoin/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDogecoin));
    assertStringsEqual(id, "doge");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDogecoin));
    assertStringsEqual(name, "Dogecoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDogecoin));
    ASSERT_EQ(0x16, TWCoinTypeP2shPrefix(TWCoinTypeDogecoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeEOS) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEOS));
    assertStringsEqual(symbol, "EOS");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEOS), 4);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEOS, txId));
    assertStringsEqual(txUrl, "https://bloks.io/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEOS));
    assertStringsEqual(id, "eos");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEOS));
    assertStringsEqual(name, "EOS");

    ASSERT_EQ(TWBlockchainEOS, TWCoinTypeBlockchain(TWCoinTypeEOS));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeEthereum) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeEthereum));
    assertStringsEqual(symbol, "ETH");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeEthereum), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeEthereum, txId));
    assertStringsEqual(txUrl, "https://etherscan.io/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeEthereum));
    assertStringsEqual(id, "ethereum");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeEthereum));
    assertStringsEqual(name, "Ethereum");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeEthereum));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeFIO) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeFIO));
    assertStringsEqual(symbol, "FIO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeFIO), 9);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeFIO, txId));
    assertStringsEqual(txUrl, "https://fio.foundation123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeFIO));
    assertStringsEqual(id, "fio");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeFIO));
    assertStringsEqual(name, "FIO");

    ASSERT_EQ(TWBlockchainFIO, TWCoinTypeBlockchain(TWCoinTypeFIO));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeGoChain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGoChain));
    assertStringsEqual(symbol, "GO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGoChain), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGoChain, txId));
    assertStringsEqual(txUrl, "https://explorer.gochain.io/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGoChain));
    assertStringsEqual(id, "gochain");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGoChain));
    assertStringsEqual(name, "GoChain");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeGoChain));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeGroestlcoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeGroestlcoin));
    assertStringsEqual(symbol, "GRS");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeGroestlcoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeGroestlcoin, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/groestlcoin/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeGroestlcoin));
    assertStringsEqual(id, "groestlcoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeGroestlcoin));
    assertStringsEqual(name, "Groestlcoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeGroestlcoin));
    ASSERT_EQ(0x05, TWCoinTypeP2shPrefix(TWCoinTypeGroestlcoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeHarmony) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeHarmony));
    assertStringsEqual(symbol, "ONE");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeHarmony), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeHarmony, txId));
    assertStringsEqual(txUrl, "https://explorer.harmony.one/#/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeHarmony));
    assertStringsEqual(id, "harmony");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeHarmony));
    assertStringsEqual(name, "Harmony");

    ASSERT_EQ(TWBlockchainHarmony, TWCoinTypeBlockchain(TWCoinTypeHarmony));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeICON) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeICON));
    assertStringsEqual(symbol, "ICX");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeICON), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeICON, txId));
    assertStringsEqual(txUrl, "https://tracker.icon.foundation/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeICON));
    assertStringsEqual(id, "icon");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeICON));
    assertStringsEqual(name, "ICON");

    ASSERT_EQ(TWBlockchainIcon, TWCoinTypeBlockchain(TWCoinTypeICON));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeIOST) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIOST));
    assertStringsEqual(symbol, "IOST");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIOST), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIOST, txId));
    assertStringsEqual(txUrl, "https://www.iostabc.com/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIOST));
    assertStringsEqual(id, "iost");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIOST));
    assertStringsEqual(name, "IOST");

    ASSERT_EQ(TWBlockchainIOST, TWCoinTypeBlockchain(TWCoinTypeIOST));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeIoTeX) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeIoTeX));
    assertStringsEqual(symbol, "IOTX");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeIoTeX), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeIoTeX, txId));
    assertStringsEqual(txUrl, "https://iotexscan.io/action/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeIoTeX));
    assertStringsEqual(id, "iotex");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeIoTeX));
    assertStringsEqual(name, "IoTeX");

    ASSERT_EQ(TWBlockchainIoTeX, TWCoinTypeBlockchain(TWCoinTypeIoTeX));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeKin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeKin));
    assertStringsEqual(symbol, "KIN");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeKin), 5);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeKin, txId));
    assertStringsEqual(txUrl, "https://kinexplorer.com/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeKin));
    assertStringsEqual(id, "kin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeKin));
    assertStringsEqual(name, "Kin");

    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeKin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeLitecoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeLitecoin));
    assertStringsEqual(symbol, "LTC");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeLitecoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeLitecoin, txId));
    assertStringsEqual(txUrl, "https://blockchair.com/litecoin/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeLitecoin));
    assertStringsEqual(id, "litecoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeLitecoin));
    assertStringsEqual(name, "Litecoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeLitecoin));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeLitecoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeMonacoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeMonacoin));
    assertStringsEqual(symbol, "MONA");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeMonacoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeMonacoin, txId));
    assertStringsEqual(txUrl, "https://blockbook.electrum-mona.org/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeMonacoin));
    assertStringsEqual(id, "monacoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeMonacoin));
    assertStringsEqual(name, "Monacoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeMonacoin));
    ASSERT_EQ(0x37, TWCoinTypeP2shPrefix(TWCoinTypeMonacoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNano) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNano));
    assertStringsEqual(symbol, "NANO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNano), 30);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNano, txId));
    assertStringsEqual(txUrl, "https://www.nanode.co/block/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNano));
    assertStringsEqual(id, "nano");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNano));
    assertStringsEqual(name, "Nano");

    ASSERT_EQ(TWBlockchainNano, TWCoinTypeBlockchain(TWCoinTypeNano));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNEAR) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEAR));
    assertStringsEqual(symbol, "NEAR");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEAR), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEAR, txId));
    assertStringsEqual(txUrl, "https://explorer.nearprotocol.com123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEAR));
    assertStringsEqual(id, "near");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEAR));
    assertStringsEqual(name, "NEAR");

    ASSERT_EQ(TWBlockchainNEAR, TWCoinTypeBlockchain(TWCoinTypeNEAR));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNebulas) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNebulas));
    assertStringsEqual(symbol, "NAS");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNebulas), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNebulas, txId));
    assertStringsEqual(txUrl, "https://explorer.nebulas.io/#/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNebulas));
    assertStringsEqual(id, "nebulas");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNebulas));
    assertStringsEqual(name, "Nebulas");

    ASSERT_EQ(TWBlockchainNebulas, TWCoinTypeBlockchain(TWCoinTypeNebulas));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNEO) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNEO));
    assertStringsEqual(symbol, "NEO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNEO), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNEO, txId));
    assertStringsEqual(txUrl, "https://neoscan.io/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNEO));
    assertStringsEqual(id, "neo");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNEO));
    assertStringsEqual(name, "NEO");

    ASSERT_EQ(TWBlockchainNEO, TWCoinTypeBlockchain(TWCoinTypeNEO));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNimiq) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNimiq));
    assertStringsEqual(symbol, "NIM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNimiq), 5);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNimiq, txId));
    assertStringsEqual(txUrl, "https://nimiq.watch/#123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNimiq));
    assertStringsEqual(id, "nimiq");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNimiq));
    assertStringsEqual(name, "Nimiq");

    ASSERT_EQ(TWBlockchainNimiq, TWCoinTypeBlockchain(TWCoinTypeNimiq));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeNULS) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeNULS));
    assertStringsEqual(symbol, "NULS");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeNULS), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeNULS, txId));
    assertStringsEqual(txUrl, "https://nulscan.io/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeNULS));
    assertStringsEqual(id, "nuls");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeNULS));
    assertStringsEqual(name, "NULS");

    ASSERT_EQ(TWBlockchainNULS, TWCoinTypeBlockchain(TWCoinTypeNULS));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeOntology) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeOntology));
    assertStringsEqual(symbol, "ONT");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeOntology), 0);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeOntology, txId));
    assertStringsEqual(txUrl, "https://explorer.ont.io/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeOntology));
    assertStringsEqual(id, "ontology");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeOntology));
    assertStringsEqual(name, "Ontology");

    ASSERT_EQ(TWBlockchainOntology, TWCoinTypeBlockchain(TWCoinTypeOntology));
}

TEST(TWCoinTypeConfiguration, TWCoinTypePOANetwork) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypePOANetwork));
    assertStringsEqual(symbol, "POA");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypePOANetwork), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypePOANetwork, txId));
    assertStringsEqual(txUrl, "https://poaexplorer.com/txid/search/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypePOANetwork));
    assertStringsEqual(id, "poa");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypePOANetwork));
    assertStringsEqual(name, "POA Network");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypePOANetwork));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeQtum) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeQtum));
    assertStringsEqual(symbol, "QTUM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeQtum), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeQtum, txId));
    assertStringsEqual(txUrl, "https://qtum.info/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeQtum));
    assertStringsEqual(id, "qtum");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeQtum));
    assertStringsEqual(name, "Qtum");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeQtum));
    ASSERT_EQ(0x32, TWCoinTypeP2shPrefix(TWCoinTypeQtum));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeRavencoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeRavencoin));
    assertStringsEqual(symbol, "RVN");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeRavencoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeRavencoin, txId));
    assertStringsEqual(txUrl, "https://ravencoin.network/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeRavencoin));
    assertStringsEqual(id, "ravencoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeRavencoin));
    assertStringsEqual(name, "Ravencoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeRavencoin));
    ASSERT_EQ(0x7a, TWCoinTypeP2shPrefix(TWCoinTypeRavencoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeXRP) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeXRP));
    assertStringsEqual(symbol, "XRP");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeXRP), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeXRP, txId));
    assertStringsEqual(txUrl, "https://bithomp.com/explorer/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeXRP));
    assertStringsEqual(id, "ripple");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeXRP));
    assertStringsEqual(name, "XRP");

    ASSERT_EQ(TWBlockchainRipple, TWCoinTypeBlockchain(TWCoinTypeXRP));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeSemux) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSemux));
    assertStringsEqual(symbol, "SEM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSemux), 9);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSemux, txId));
    assertStringsEqual(txUrl, "https://semux.info/explorer/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSemux));
    assertStringsEqual(id, "semux");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSemux));
    assertStringsEqual(name, "Semux");

    ASSERT_EQ(TWBlockchainSemux, TWCoinTypeBlockchain(TWCoinTypeSemux));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeSolana) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSolana));
    assertStringsEqual(symbol, "SOL");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSolana), 13);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSolana, txId));
    assertStringsEqual(txUrl, "https://explorer.solana.com/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSolana));
    assertStringsEqual(id, "solana");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSolana));
    assertStringsEqual(name, "Solana");

    ASSERT_EQ(TWBlockchainSolana, TWCoinTypeBlockchain(TWCoinTypeSolana));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeSteem) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeSteem));
    assertStringsEqual(symbol, "STEEM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeSteem), 3);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeSteem, txId));
    assertStringsEqual(txUrl, "https://steemblockexplorer.com/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeSteem));
    assertStringsEqual(id, "steem");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeSteem));
    assertStringsEqual(name, "Steem");

    ASSERT_EQ(TWBlockchainSteem, TWCoinTypeBlockchain(TWCoinTypeSteem));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeStellar) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeStellar));
    assertStringsEqual(symbol, "XLM");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeStellar), 7);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeStellar, txId));
    assertStringsEqual(txUrl, "https://stellarscan.io/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeStellar));
    assertStringsEqual(id, "stellar");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeStellar));
    assertStringsEqual(name, "Stellar");

    ASSERT_EQ(TWBlockchainStellar, TWCoinTypeBlockchain(TWCoinTypeStellar));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeTerra) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTerra));
    assertStringsEqual(symbol, "LUNA");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTerra), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTerra, txId));
    assertStringsEqual(txUrl, "https://terra.stake.id/?#/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTerra));
    assertStringsEqual(id, "terra");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTerra));
    assertStringsEqual(name, "Terra");

    ASSERT_EQ(TWBlockchainCosmos, TWCoinTypeBlockchain(TWCoinTypeTerra));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeTezos) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTezos));
    assertStringsEqual(symbol, "XTZ");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTezos), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTezos, txId));
    assertStringsEqual(txUrl, "https://tezos.id/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTezos));
    assertStringsEqual(id, "tezos");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTezos));
    assertStringsEqual(name, "Tezos");

    ASSERT_EQ(TWBlockchainTezos, TWCoinTypeBlockchain(TWCoinTypeTezos));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeTheta) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTheta));
    assertStringsEqual(symbol, "THETA");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTheta), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTheta, txId));
    assertStringsEqual(txUrl, "https://explorer.thetatoken.org/txs/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTheta));
    assertStringsEqual(id, "theta");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTheta));
    assertStringsEqual(name, "Theta");

    ASSERT_EQ(TWBlockchainTheta, TWCoinTypeBlockchain(TWCoinTypeTheta));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeThunderToken) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeThunderToken));
    assertStringsEqual(symbol, "TT");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeThunderToken), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeThunderToken, txId));
    assertStringsEqual(txUrl, "https://scan.thundercore.com/transactions/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeThunderToken));
    assertStringsEqual(id, "thundertoken");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeThunderToken));
    assertStringsEqual(name, "Thunder Token");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeThunderToken));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeTomoChain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTomoChain));
    assertStringsEqual(symbol, "TOMO");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTomoChain), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTomoChain, txId));
    assertStringsEqual(txUrl, "https://scan.tomochain.com/txs/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTomoChain));
    assertStringsEqual(id, "tomochain");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTomoChain));
    assertStringsEqual(name, "TomoChain");

    ASSERT_EQ(TWBlockchainEthereum, TWCoinTypeBlockchain(TWCoinTypeTomoChain));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeTron) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeTron));
    assertStringsEqual(symbol, "TRX");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeTron), 6);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeTron, txId));
    assertStringsEqual(txUrl, "https://tronscan.org/#/transaction/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeTron));
    assertStringsEqual(id, "tron");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeTron));
    assertStringsEqual(name, "Tron");

    ASSERT_EQ(TWBlockchainTron, TWCoinTypeBlockchain(TWCoinTypeTron));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeVeChain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeVeChain));
    assertStringsEqual(symbol, "VET");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeVeChain), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeVeChain, txId));
    assertStringsEqual(txUrl, "https://insight.vecha.in/#/txs/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeVeChain));
    assertStringsEqual(id, "vechain");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeVeChain));
    assertStringsEqual(name, "VeChain");

    ASSERT_EQ(TWBlockchainVechain, TWCoinTypeBlockchain(TWCoinTypeVeChain));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeViacoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeViacoin));
    assertStringsEqual(symbol, "VIA");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeViacoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeViacoin, txId));
    assertStringsEqual(txUrl, "https://explorer.viacoin.org/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeViacoin));
    assertStringsEqual(id, "viacoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeViacoin));
    assertStringsEqual(name, "Viacoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeViacoin));
    ASSERT_EQ(0x21, TWCoinTypeP2shPrefix(TWCoinTypeViacoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeWanchain) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWanchain));
    assertStringsEqual(symbol, "WAN");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWanchain), 18);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWanchain, txId));
    assertStringsEqual(txUrl, "https://explorer.wanchain.org/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWanchain));
    assertStringsEqual(id, "wanchain");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWanchain));
    assertStringsEqual(name, "Wanchain");

    ASSERT_EQ(TWBlockchainWanchain, TWCoinTypeBlockchain(TWCoinTypeWanchain));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeWaves) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeWaves));
    assertStringsEqual(symbol, "WAVES");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeWaves), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeWaves, txId));
    assertStringsEqual(txUrl, "https://wavesexplorer.com/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeWaves));
    assertStringsEqual(id, "waves");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeWaves));
    assertStringsEqual(name, "Waves");

    ASSERT_EQ(TWBlockchainWaves, TWCoinTypeBlockchain(TWCoinTypeWaves));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeZcash) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcash));
    assertStringsEqual(symbol, "ZEC");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcash), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcash, txId));
    assertStringsEqual(txUrl, "https://chain.so/tx/ZEC/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcash));
    assertStringsEqual(id, "zcash");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcash));
    assertStringsEqual(name, "Zcash");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcash));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZcash));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeZcoin) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZcoin));
    assertStringsEqual(symbol, "XZC");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZcoin), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZcoin, txId));
    assertStringsEqual(txUrl, "https://explorer.zcoin.io/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZcoin));
    assertStringsEqual(id, "zcoin");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZcoin));
    assertStringsEqual(name, "Zcoin");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZcoin));
    ASSERT_EQ(0x07, TWCoinTypeP2shPrefix(TWCoinTypeZcoin));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeZelcash) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZelcash));
    assertStringsEqual(symbol, "ZEL");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZelcash), 8);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZelcash, txId));
    assertStringsEqual(txUrl, "https://explorer.zel.cash/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZelcash));
    assertStringsEqual(id, "zelcash");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZelcash));
    assertStringsEqual(name, "Zelcash");

    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeZelcash));
    ASSERT_EQ(0xbd, TWCoinTypeP2shPrefix(TWCoinTypeZelcash));
    ASSERT_EQ(0x1c, TWCoinTypeStaticPrefix(TWCoinTypeZelcash));
}

TEST(TWCoinTypeConfiguration, TWCoinTypeZilliqa) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeZilliqa));
    assertStringsEqual(symbol, "ZIL");

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeZilliqa), 12);

    auto txId = TWStringCreateWithUTF8Bytes("123");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeZilliqa, txId));
    assertStringsEqual(txUrl, "https://viewblock.io/zilliqa/tx/123");

    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeZilliqa));
    assertStringsEqual(id, "zilliqa");

    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeZilliqa));
    assertStringsEqual(name, "Zilliqa");

    ASSERT_EQ(TWBlockchainZilliqa, TWCoinTypeBlockchain(TWCoinTypeZilliqa));
}
