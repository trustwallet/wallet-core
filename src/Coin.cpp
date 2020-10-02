// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "CoinEntry.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <TrustWalletCore/TWHRP.h>

#include <map>

// #coin-list# Includes for entry points for coin implementations
#include "Aeternity/Entry.h"
#include "Aion/Entry.h"
#include "Algorand/Entry.h"
#include "Bitcoin/Entry.h"
#include "Binance/Entry.h"
#include "Cardano/Entry.h"
#include "Cosmos/Entry.h"
#include "Decred/Entry.h"
#include "Elrond/Entry.h"
#include "EOS/Entry.h"
#include "Ethereum/Entry.h"
#include "Filecoin/Entry.h"
#include "FIO/Entry.h"
#include "Groestlcoin/Entry.h"
#include "Harmony/Entry.h"
#include "Icon/Entry.h"
#include "IoTeX/Entry.h"
#include "Kusama/Entry.h"
#include "Nano/Entry.h"
#include "NEAR/Entry.h"
#include "Nebulas/Entry.h"
#include "NEO/Entry.h"
#include "Nimiq/Entry.h"
#include "NULS/Entry.h"
#include "Ontology/Entry.h"
#include "Polkadot/Entry.h"
#include "Ripple/Entry.h"
#include "Solana/Entry.h"
#include "Stellar/Entry.h"
#include "Tezos/Entry.h"
#include "Theta/Entry.h"
#include "TON/Entry.h"
#include "Tron/Entry.h"
#include "VeChain/Entry.h"
#include "Wanchain/Entry.h"
#include "Waves/Entry.h"
#include "Zcash/Entry.h"
#include "Zilliqa/Entry.h"
// end_of_coin_includes_marker_do_not_modify

using namespace TW;
using namespace std;

// #coin-list# Global coin entry dispatcher entries
Aeternity::Entry aeternityDP;
Aion::Entry aionDP;
Algorand::Entry algorandDP;
Binance::Entry binanceDP;
Bitcoin::Entry bitcoinDP;
Cardano::Entry cardanoDP;
Cosmos::Entry cosmosDP;
Elrond::Entry elrondDP;
EOS::Entry eosDP;
Ethereum::Entry ethereumDP;
Decred::Entry decredDP;
Filecoin::Entry filecoinDP;
FIO::Entry fioDP;
Groestlcoin::Entry groestlcoinDP;
Harmony::Entry harmonyDP;
Icon::Entry iconDP;
IoTeX::Entry iotexDP;
Kusama::Entry kusamaDP;
Nano::Entry nanoDP;
NEAR::Entry nearDP;
Nebulas::Entry nebulasDP;
NEO::Entry neoDP;
Nimiq::Entry nimiqDP;
NULS::Entry nulsDP;
Ontology::Entry ontologyDP;
Polkadot::Entry polkadotDP;
Ripple::Entry rippleDP;
Solana::Entry solanaDP;
Stellar::Entry stellarDP;
Tezos::Entry tezosDP;
Theta::Entry thetaDP;
TON::Entry tonDP;
Tron::Entry tronDP;
VeChain::Entry vechainDP;
Wanchain::Entry wanchainDP;
Waves::Entry wavesDP;
Zcash::Entry zcashDP;
Zilliqa::Entry zilliqaDP;
// end_of_coin_dipatcher_declarations_marker_do_not_modify

CoinEntry* coinDispatcher(TWCoinType coinType) {
    // switch is preferred instead of a data structure, due to initialization issues
    switch (coinType) {
        // #coin-list#
        case TWCoinTypeAeternity: return &aeternityDP;
        case TWCoinTypeAion: return &aionDP;
        case TWCoinTypeAlgorand: return &algorandDP;
        case TWCoinTypeBinance: return &binanceDP;
        case TWCoinTypeBitcoin: return &bitcoinDP;
        case TWCoinTypeBitcoinCash: return &bitcoinDP;
        case TWCoinTypeBitcoinGold: return &bitcoinDP;
        case TWCoinTypeDash: return &bitcoinDP;
        case TWCoinTypeDigiByte: return &bitcoinDP;
        case TWCoinTypeDogecoin: return &bitcoinDP;
        case TWCoinTypeLitecoin: return &bitcoinDP;
        case TWCoinTypeMonacoin: return &bitcoinDP;
        case TWCoinTypeQtum: return &bitcoinDP;
        case TWCoinTypeRavencoin: return &bitcoinDP;
        case TWCoinTypeViacoin: return &bitcoinDP;
        case TWCoinTypeZcoin: return &bitcoinDP;
        case TWCoinTypeCardano: return &cardanoDP;
        case TWCoinTypeCosmos: return &cosmosDP;
        case TWCoinTypeKava: return &cosmosDP;
        case TWCoinTypeTerra: return &cosmosDP;
        case TWCoinTypeBandChain: return &cosmosDP;
        case TWCoinTypeElrond: return &elrondDP;
        case TWCoinTypeEOS: return &eosDP;
        case TWCoinTypeCallisto: return &ethereumDP;
        case TWCoinTypeEthereum: return &ethereumDP;
        case TWCoinTypeEthereumClassic: return &ethereumDP;
        case TWCoinTypeGoChain: return &ethereumDP;
        case TWCoinTypePOANetwork: return &ethereumDP;
        case TWCoinTypeThunderToken: return &ethereumDP;
        case TWCoinTypeTomoChain: return &ethereumDP;
        case TWCoinTypeSmartChainLegacy: return &ethereumDP;
        case TWCoinTypeSmartChain: return &ethereumDP;
        case TWCoinTypeDecred: return &decredDP;
        case TWCoinTypeFilecoin: return &filecoinDP;
        case TWCoinTypeFIO: return &fioDP;
        case TWCoinTypeGroestlcoin: return &groestlcoinDP;
        case TWCoinTypeHarmony: return &harmonyDP;
        case TWCoinTypeICON: return &iconDP;
        case TWCoinTypeIoTeX: return &iotexDP;
        case TWCoinTypeKusama: return &kusamaDP;
        case TWCoinTypeNano: return &nanoDP;
        case TWCoinTypeNEAR: return &nearDP;
        case TWCoinTypeNebulas: return &nebulasDP;
        case TWCoinTypeNEO: return &neoDP;
        case TWCoinTypeNimiq: return &nimiqDP;
        case TWCoinTypeNULS: return &nulsDP;
        case TWCoinTypeOntology: return &ontologyDP;
        case TWCoinTypePolkadot: return &polkadotDP;
        case TWCoinTypeXRP: return &rippleDP;
        case TWCoinTypeSolana: return &solanaDP;
        case TWCoinTypeStellar: return &stellarDP;
        case TWCoinTypeKin: return &stellarDP;
        case TWCoinTypeTezos: return &tezosDP;
        case TWCoinTypeTheta: return &thetaDP;
        case TWCoinTypeTON: return &tonDP;
        case TWCoinTypeTron: return &tronDP;
        case TWCoinTypeVeChain: return &vechainDP;
        case TWCoinTypeWanchain: return &wanchainDP;
        case TWCoinTypeWaves: return &wavesDP;
        case TWCoinTypeZcash: return &zcashDP;
        case TWCoinTypeZelcash: return &zcashDP;
        case TWCoinTypeZilliqa: return &zilliqaDP;
// end_of_coin_dipatcher_switch_marker_do_not_modify

        default:
            assert(false);
            return nullptr;
    }
}

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto p2sh = TW::p2shPrefix(coin);
    auto hrp = stringForHRP(TW::hrp(coin));

    // dispatch
    auto dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->validateAddress(coin, string, p2pkh, p2sh, hrp);
}

std::string TW::normalizeAddress(TWCoinType coin, const std::string& address) {
    if (!TW::validateAddress(coin, address)) {
        // invalid address, not normalizing
        return "";
    }

    // dispatch
    auto dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->normalizeAddress(coin, address);
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    auto keyType = TW::publicKeyType(coin);
    return TW::deriveAddress(coin, privateKey.getPublicKey(keyType));
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey& publicKey) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto hrp = stringForHRP(TW::hrp(coin));

    // dispatch
    auto dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->deriveAddress(coin, publicKey, p2pkh, hrp);
}

void TW::anyCoinSign(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->sign(coinType, dataIn, dataOut);
}

std::string TW::anySignJSON(TWCoinType coinType, const std::string& json, const Data& key) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->signJSON(coinType, json, key);
}

bool TW::supportsJSONSigning(TWCoinType coinType) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->supportsJSONSigning();
}

void TW::anyCoinEncode(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->encodeRawTx(coinType, dataIn, dataOut);
}

void TW::anyCoinDecode(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->decodeRawTx(coinType, dataIn, dataOut);
}

void TW::anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->plan(coinType, dataIn, dataOut);
}

// Coin info accessors

extern const CoinInfo getCoinInfo(TWCoinType coin); // in generated CoinInfoData.cpp file

TWBlockchain TW::blockchain(TWCoinType coin) {
    return getCoinInfo(coin).blockchain;
}

TWPurpose TW::purpose(TWCoinType coin) {
    return getCoinInfo(coin).purpose;
}

TWCurve TW::curve(TWCoinType coin) {
    return getCoinInfo(coin).curve;
}

TWHDVersion TW::xpubVersion(TWCoinType coin) {
    return getCoinInfo(coin).xpubVersion;
}

TWHDVersion TW::xprvVersion(TWCoinType coin) {
    return getCoinInfo(coin).xprvVersion;
}

DerivationPath TW::derivationPath(TWCoinType coin) {
    return getCoinInfo(coin).derivationPath;
}

enum TWPublicKeyType TW::publicKeyType(TWCoinType coin) {
    return getCoinInfo(coin).publicKeyType;
}

TW::byte TW::staticPrefix(TWCoinType coin) {
    return getCoinInfo(coin).staticPrefix;
}

TW::byte TW::p2pkhPrefix(TWCoinType coin) {
    return getCoinInfo(coin).p2pkhPrefix;
}

TW::byte TW::p2shPrefix(TWCoinType coin) {
    return getCoinInfo(coin).p2shPrefix;
}

enum TWHRP TW::hrp(TWCoinType coin) {
    return getCoinInfo(coin).hrp;
}

Hash::Hasher TW::publicKeyHasher(TWCoinType coin) {
    return getCoinInfo(coin).publicKeyHasher;
}

Hash::Hasher TW::base58Hasher(TWCoinType coin) {
    return getCoinInfo(coin).base58Hasher;
}

uint32_t TW::slip44Id(TWCoinType coin) {
    return getCoinInfo(coin).slip44;
}

TWString *_Nullable TWCoinTypeConfigurationGetSymbol(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).symbol);
}

int TWCoinTypeConfigurationGetDecimals(enum TWCoinType coin) {
    return getCoinInfo(coin).decimals;
}

TWString *_Nullable TWCoinTypeConfigurationGetTransactionURL(enum TWCoinType coin, TWString *_Nonnull transactionID) {
    std::string txId = TWStringUTF8Bytes(transactionID);
    std::string url = getCoinInfo(coin).explorerTransactionUrl + txId;
    return TWStringCreateWithUTF8Bytes(url.c_str());
}

TWString *_Nullable TWCoinTypeConfigurationGetAccountURL(enum TWCoinType coin, TWString *_Nonnull accountID) {
    std::string accId = TWStringUTF8Bytes(accountID);
    std::string url = getCoinInfo(coin).explorerAccountUrl + accId;
    return TWStringCreateWithUTF8Bytes(url.c_str());
}

TWString *_Nonnull TWCoinTypeConfigurationGetID(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).id);
}

TWString *_Nonnull TWCoinTypeConfigurationGetName(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).name);
}
