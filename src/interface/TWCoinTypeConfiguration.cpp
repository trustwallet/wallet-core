// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace std;

const char *explorerURLForCoinType(enum TWCoinType type);

TWString *_Nullable TWCoinTypeConfigurationGetSymbol(enum TWCoinType type) {
    string string;
    switch (type) {
    case TWCoinTypeEthereum: string = "ETH"; break;
    case TWCoinTypeBitcoin: string = "BTC"; break;
    case TWCoinTypeBitcoinCash:  string = "BCH"; break;
    case TWCoinTypeCallisto:  string = "CLO"; break;
    case TWCoinTypeDash:  string = "DASH"; break;
    case TWCoinTypeDecred: string = "DCR"; break;
    case TWCoinTypeDogecoin: string = "DOGE"; break;
    case TWCoinTypeEthereumClassic:  string = "ETC"; break;
    case TWCoinTypeGo:  string =  "GO"; break;
    case TWCoinTypeGroestlcoin: string = "GRS"; break;
    case TWCoinTypeICON:  string =  "ICX"; break;
    case TWCoinTypeIOST:  string = "IOST"; break;
    case TWCoinTypeLitecoin: string = "LTC"; break;
    case TWCoinTypeOntology: string = "ONT"; break;
    case TWCoinTypeViacoin: string = "VIA"; break;
    case TWCoinTypePoa: string = "POA"; break;
    case TWCoinTypeThunderToken: string = "TT"; break;
    case TWCoinTypeTomoChain: string = "TOMO"; break;
    case TWCoinTypeTron: string = "TRX"; break;
    case TWCoinTypeVeChain: string = "VET"; break;
    case TWCoinTypeWanChain: string = "WAN"; break;
    case TWCoinTypeXDai: string = "xDAI"; break;
    case TWCoinTypeZcoin: string = "XZC"; break;
    case TWCoinTypeZcash: string = "ZEC"; break;
    case TWCoinTypeBinance: string = "BNB"; break;
    case TWCoinTypeRipple: string = "XRP"; break;
    case TWCoinTypeTezos: string = "XTZ"; break;
    case TWCoinTypeNimiq: string = "NIM"; break;
    case TWCoinTypeStellar: string = "XLM"; break;
    case TWCoinTypeAion: string = "AION"; break;
    case TWCoinTypeCosmos: string = "ATOM"; break;
    case TWCoinTypeNEO: string = "NEO"; break;
    case TWCoinTypeKIN: string = "KIN"; break;
    case TWCoinTypeTheta: string = "THETA"; break;
    case TWCoinTypeQtum: string = "QTUM"; break;
    default: string = ""; break;
    }
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

int TWCoinTypeConfigurationGetDecimals(enum TWCoinType type) {
    switch (type) {
    case TWCoinTypeEthereum:
    case TWCoinTypeCallisto:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeICON:
    case TWCoinTypeGo:
    case TWCoinTypePoa:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeAion:
    case TWCoinTypeTheta:
        return 18;
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeBitcoin:
    case TWCoinTypeDash:
    case TWCoinTypeDecred:
    case TWCoinTypeDogecoin:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeIOST:
    case TWCoinTypeLitecoin:
    case TWCoinTypeViacoin:
    case TWCoinTypeBinance:
    case TWCoinTypeZcoin:
    case TWCoinTypeZcash:
    case TWCoinTypeNEO:
    case TWCoinTypeQtum:
        return 8;
    case TWCoinTypeStellar:
        return 7;
    case TWCoinTypeRipple:
    case TWCoinTypeTezos:
    case TWCoinTypeTron:
    case TWCoinTypeCosmos:
        return 6;
    case TWCoinTypeNimiq:
    case TWCoinTypeKIN:
        return 5;
    case TWCoinTypeOntology:
        return 0;
    }
}

TWString *_Nullable TWCoinTypeConfigurationGetTransactionURL(enum TWCoinType type, TWString *_Nonnull transactionID) {
    string url = string(explorerURLForCoinType(type));
    string txId = string(TWStringUTF8Bytes(transactionID));
    switch (type) {
    case TWCoinTypeBitcoin:
    case TWCoinTypeBitcoinCash:
    case TWCoinTypeDash:
    case TWCoinTypeICON:
    case TWCoinTypeLitecoin:
    case TWCoinTypeNEO:
    case TWCoinTypeStellar:
    case TWCoinTypeOntology:
        url += "/transaction/" + txId;
        break;
    case TWCoinTypeCallisto:
    case TWCoinTypeDecred:
    case TWCoinTypeDogecoin:
    case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGo:
    case TWCoinTypeGroestlcoin:
    case TWCoinTypeKIN:
    case TWCoinTypeIOST:
    case TWCoinTypeViacoin:
    case TWCoinTypeWanChain:
    case TWCoinTypeXDai:
    case TWCoinTypeZcoin:
    case TWCoinTypeQtum:
        url += "/tx/" + txId;
        break;
    case TWCoinTypePoa:
        url += "/txid/search/" + txId;
        break;
    case TWCoinTypeTomoChain:
    case TWCoinTypeTheta:
        url += "/txs/" + txId;
        break;
    case TWCoinTypeTron:
    case TWCoinTypeAion:
        url += "/#/transaction/" + txId;
        break;
    case TWCoinTypeVeChain:
    case TWCoinTypeThunderToken:
        url += "/transactions/" + txId;
        break;
    case TWCoinTypeZcash:
        url += "/tx/ZEC/" + txId;
        break;
    case TWCoinTypeRipple:
        url += "/explorer/" + txId;
        break;
    case TWCoinTypeTezos:
        url += "/" + txId;
        break;
    case TWCoinTypeNimiq:
        url += "/#" + txId;
        break;
    case TWCoinTypeBinance: break;
    case TWCoinTypeCosmos:
        url += "/blocks/1/transactions/" + txId;
        break;
    default: break;
    }
    return TWStringCreateWithUTF8Bytes(url.c_str());
}

const char *explorerURLForCoinType(enum TWCoinType type) {
    switch (type) {
    case TWCoinTypeEthereum: return "https://etherscan.io";
    case TWCoinTypeBitcoin: return "https://blockchair.com/bitcoin";
    case TWCoinTypeBitcoinCash: return "https://blockchair.com/bitcoin-cash";
    case TWCoinTypeCallisto: return "https://explorer2.callisto.network";
    case TWCoinTypeDash: return "https://blockchair.com/dash";
    case TWCoinTypeDecred: return "https://mainnet.decred.org";
    case TWCoinTypeDogecoin: return "https://live.blockcypher.com/doge";
    case TWCoinTypeEthereumClassic: return "https://gastracker.io";
    case TWCoinTypeGo: return "https://explorer.gochain.io";
    case TWCoinTypeGroestlcoin: return "https://blockbook.groestlcoin.org";
    case TWCoinTypeICON: return "https://tracker.icon.foundation";
    case TWCoinTypeIOST: return "https://www.iostabc.com";
    case TWCoinTypeLitecoin: return "https://blockchair.com/litecoin";
    case TWCoinTypeOntology: return "https://explorer.ont.io";
    case TWCoinTypeViacoin: return "https://explorer.viacoin.org";
    case TWCoinTypePoa: return "https://poaexplorer.com";
    case TWCoinTypeThunderToken: return "https://scan.thundercore.com";
    case TWCoinTypeTomoChain: return "https://scan.tomochain.com";
    case TWCoinTypeTron: return "https://tronscan.org";
    case TWCoinTypeVeChain: return "https://explore.veforge.com";
    case TWCoinTypeWanChain: return "https://explorer.wanchain.org";
    case TWCoinTypeXDai: return "https://blockscout.com/poa/dai";
    case TWCoinTypeZcoin: return "https://explorer.zcoin.io";
    case TWCoinTypeZcash: return "https://chain.so";
    case TWCoinTypeBinance: return "https://binance.com";
    case TWCoinTypeRipple: return "https://bithomp.com";
    case TWCoinTypeTezos: return "https://tzscan.io";
    case TWCoinTypeNimiq: return "https://nimiq.watch";
    case TWCoinTypeStellar: return "https://stellarscan.io";
    case TWCoinTypeAion: return "https://mainnet.aion.network";
    case TWCoinTypeCosmos: return "https://hubble.figment.network/chains/cosmoshub-1";
    case TWCoinTypeNEO: return "https://neoscan.io";
    case TWCoinTypeKIN: return "https://kinexplorer.com";
    case TWCoinTypeTheta: return "https://explorer.thetatoken.org";
    case TWCoinTypeQtum: return "https://qtum.info";
    default: return "";
    }
}

TWString *_Nonnull TWCoinTypeConfigurationGetID(enum TWCoinType type) {
    string string;
    switch (type) {
    case TWCoinTypeEthereum: string = "ethereum"; break;
    case TWCoinTypeBitcoin: string = "bitcoin"; break;
    case TWCoinTypeBitcoinCash:  string = "bitcoincash"; break;
    case TWCoinTypeCallisto:  string = "callisto"; break;
    case TWCoinTypeDash:  string = "dash"; break;
    case TWCoinTypeDecred: string = "decred"; break;
    case TWCoinTypeDogecoin: string = "doge"; break;
    case TWCoinTypeEthereumClassic:  string = "classic"; break;
    case TWCoinTypeGo:  string =  "gochain"; break;
    case TWCoinTypeGroestlcoin: string = "groestlcoin"; break;
    case TWCoinTypeICON:  string =  "icon"; break;
    case TWCoinTypeIOST:  string =  "iost"; break;
    case TWCoinTypeLitecoin: string = "litecoin"; break;
    case TWCoinTypeOntology: string = "ontology"; break;
    case TWCoinTypeViacoin: string = "viacoin"; break;
    case TWCoinTypePoa: string = "poa"; break;
    case TWCoinTypeThunderToken: string = "thundertoken"; break;
    case TWCoinTypeTomoChain: string = "tomochain"; break;
    case TWCoinTypeTron: string = "tron"; break;
    case TWCoinTypeVeChain: string = "vechain"; break;
    case TWCoinTypeWanChain: string = "wanchain"; break;
    case TWCoinTypeXDai: string = "xdai"; break;
    case TWCoinTypeZcoin: string = "zcoin"; break;
    case TWCoinTypeZcash: string = "zcash"; break;
    case TWCoinTypeBinance: string = "binance"; break;
    case TWCoinTypeRipple: string = "ripple"; break;
    case TWCoinTypeTezos: string = "tezos"; break;
    case TWCoinTypeNimiq: string = "nimiq"; break;
    case TWCoinTypeStellar: string = "stellar"; break;
    case TWCoinTypeAion: string = "aion"; break;
    case TWCoinTypeCosmos: string = "cosmos"; break;
    case TWCoinTypeNEO: string = "neo"; break;
    case TWCoinTypeKIN: string = "kin"; break;
    case TWCoinTypeTheta: string = "theta"; break;
    case TWCoinTypeQtum: string = "qtum"; break;
    default: string = ""; break;
    }
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWString *_Nonnull TWCoinTypeConfigurationGetName(enum TWCoinType type) {
    string string;
    switch (type) {
    case TWCoinTypeEthereum: string = "Ethereum"; break;
    case TWCoinTypeBitcoin: string = "Bitcoin"; break;
    case TWCoinTypeBitcoinCash:  string = "Bitcoin Cash"; break;
    case TWCoinTypeCallisto:  string = "Callisto"; break;
    case TWCoinTypeDash:  string = "Dash"; break;
    case TWCoinTypeDecred: string = "Decred"; break;
    case TWCoinTypeDogecoin: string = "Dogecoin"; break;
    case TWCoinTypeEthereumClassic:  string = "Ethereum Classic"; break;
    case TWCoinTypeGo:  string =  "GoChain"; break;
    case TWCoinTypeGroestlcoin: string =  "Groestlcoin"; break;
    case TWCoinTypeICON:  string =  "ICON"; break;
    case TWCoinTypeIOST:  string =  "IOST"; break;
    case TWCoinTypeLitecoin: string = "Litecoin"; break;
    case TWCoinTypeOntology: string = "Ontology"; break;
    case TWCoinTypeViacoin: string = "Viacoin"; break;
    case TWCoinTypePoa: string = "POA Network"; break;
    case TWCoinTypeThunderToken: string = "Thunder Token"; break;
    case TWCoinTypeTomoChain: string = "TomoChain"; break;
    case TWCoinTypeTron: string = "Tron"; break;
    case TWCoinTypeVeChain: string = "VeChain"; break;
    case TWCoinTypeWanChain: string = "Wanchain"; break;
    case TWCoinTypeXDai: string = "xDai"; break;
    case TWCoinTypeZcoin: string = "Zcoin"; break;
    case TWCoinTypeZcash: string = "Zcash"; break;
    case TWCoinTypeBinance: string = "Binance"; break;
    case TWCoinTypeRipple: string = "XRP"; break;
    case TWCoinTypeTezos: string = "Tezos"; break;
    case TWCoinTypeNimiq: string = "Nimiq"; break;
    case TWCoinTypeStellar: string = "Stellar"; break;
    case TWCoinTypeAion: string = "Aion"; break;
    case TWCoinTypeCosmos: string = "Cosmos"; break;
    case TWCoinTypeNEO: string = "NEO"; break;
    case TWCoinTypeKIN: string = "Kin"; break;
    case TWCoinTypeTheta: string = "Theta"; break;
    case TWCoinTypeQtum: string = "Qtum"; break;
    default: string = ""; break;
    }
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

#pragma clang diagnostic pop
