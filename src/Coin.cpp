// Copyright © 2017-2022 Trust Wallet.
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
#include "Binance/Entry.h"
#include "Bitcoin/Entry.h"
#include "Cardano/Entry.h"
#include "Cosmos/Entry.h"
#include "Decred/Entry.h"
#include "EOS/Entry.h"
#include "Elrond/Entry.h"
#include "Ethereum/Entry.h"
#include "FIO/Entry.h"
#include "Filecoin/Entry.h"
#include "Groestlcoin/Entry.h"
#include "Harmony/Entry.h"
#include "Icon/Entry.h"
#include "IoTeX/Entry.h"
#include "Kusama/Entry.h"
#include "NEAR/Entry.h"
#include "NEO/Entry.h"
#include "NULS/Entry.h"
#include "Nano/Entry.h"
#include "Nebulas/Entry.h"
#include "Nimiq/Entry.h"
#include "Oasis/Entry.h"
#include "Ontology/Entry.h"
#include "Polkadot/Entry.h"
#include "Ripple/Entry.h"
#include "Ronin/Entry.h"
#include "Solana/Entry.h"
#include "Stellar/Entry.h"
#include "THORChain/Entry.h"
#include "Tezos/Entry.h"
#include "Theta/Entry.h"
#include "Tron/Entry.h"
#include "VeChain/Entry.h"
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
Oasis::Entry oasisDP;
Polkadot::Entry polkadotDP;
Ripple::Entry rippleDP;
Ronin::Entry roninDP;
Solana::Entry solanaDP;
Stellar::Entry stellarDP;
Tezos::Entry tezosDP;
Theta::Entry thetaDP;
THORChain::Entry thorchainDP;
Tron::Entry tronDP;
VeChain::Entry vechainDP;
Waves::Entry wavesDP;
Zcash::Entry zcashDP;
Zilliqa::Entry zilliqaDP;
// end_of_coin_dipatcher_declarations_marker_do_not_modify

CoinEntry* coinDispatcher(TWCoinType coinType) {
    // switch is preferred instead of a data structure, due to initialization issues
    CoinEntry* entry = nullptr;
    const auto blockchain = TW::blockchain(coinType);
    switch (blockchain) {
        // #coin-list#
        case TWBlockchainBitcoin: entry = &bitcoinDP; break;
        case TWBlockchainEthereum: entry = &ethereumDP; break;
        case TWBlockchainVechain: entry = &vechainDP; break;
        case TWBlockchainTron: entry = &tronDP; break;
        case TWBlockchainIcon: entry = &iconDP; break;
        case TWBlockchainBinance: entry = &binanceDP; break;
        case TWBlockchainRipple: entry = &rippleDP; break;
        case TWBlockchainTezos: entry = &tezosDP; break;
        case TWBlockchainNimiq: entry = &nimiqDP; break;
        case TWBlockchainStellar: entry = &stellarDP; break;
        case TWBlockchainAion: entry = &aionDP; break;
        case TWBlockchainCosmos: entry = &cosmosDP; break;
        case TWBlockchainTheta: entry = &thetaDP; break;
        case TWBlockchainOntology: entry = &ontologyDP; break;
        case TWBlockchainZilliqa: entry = &zilliqaDP; break;
        case TWBlockchainIoTeX: entry = &iotexDP; break;
        case TWBlockchainEOS: entry = &eosDP; break;
        case TWBlockchainNano: entry = &nanoDP; break;
        case TWBlockchainNULS: entry = &nulsDP; break;
        case TWBlockchainWaves: entry = &wavesDP; break;
        case TWBlockchainAeternity: entry = &aeternityDP; break;
        case TWBlockchainNebulas: entry = &nebulasDP; break;
        case TWBlockchainFIO: entry = &fioDP; break;
        case TWBlockchainSolana: entry = &solanaDP; break;
        case TWBlockchainHarmony: entry = &harmonyDP; break;
        case TWBlockchainNEAR: entry = &nearDP; break;
        case TWBlockchainAlgorand: entry = &algorandDP; break;
        case TWBlockchainPolkadot: entry = &polkadotDP; break;
        case TWBlockchainCardano: entry = &cardanoDP; break;
        case TWBlockchainNEO: entry = &neoDP; break;
        case TWBlockchainFilecoin: entry = &filecoinDP; break;
        case TWBlockchainElrondNetwork: entry = &elrondDP; break;
        case TWBlockchainOasisNetwork: entry = &oasisDP; break;
        case TWBlockchainDecred: entry = &decredDP; break;
        case TWBlockchainGroestlcoin: entry = &groestlcoinDP; break;
        case TWBlockchainZcash: entry = &zcashDP; break;
        case TWBlockchainThorchain: entry = &thorchainDP; break;
        case TWBlockchainRonin: entry = &roninDP; break;
        case TWBlockchainKusama: entry = &kusamaDP; break;
        // end_of_coin_dipatcher_switch_marker_do_not_modify

        default: entry = nullptr; break;
    }
    assert(entry != nullptr);
    return entry;
}

const Derivation CoinInfo::derivationByName(TWDerivation name) const {
    if (name == TWDerivationDefault && derivation.size() > 0) {
        return derivation[0];
    }
    for (auto deriv: derivation) {
        if (deriv.name == name) {
            return deriv;
        }
    }
    return Derivation();
}

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto p2sh = TW::p2shPrefix(coin);
    const auto* hrp = stringForHRP(TW::hrp(coin));

    // dispatch
    auto* dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->validateAddress(coin, string, p2pkh, p2sh, hrp);
}

std::string TW::normalizeAddress(TWCoinType coin, const std::string& address) {
    if (!TW::validateAddress(coin, address)) {
        // invalid address, not normalizing
        return "";
    }

    // dispatch
    auto* dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->normalizeAddress(coin, address);
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey) {
    return TW::deriveAddress(coin, privateKey, TWDerivationDefault);
}

std::string TW::deriveAddress(TWCoinType coin, const PrivateKey& privateKey, TWDerivation derivation) {
    auto keyType = TW::publicKeyType(coin);
    return TW::deriveAddress(coin, privateKey.getPublicKey(keyType), derivation);
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey& publicKey) {
    return deriveAddress(coin, publicKey, TWDerivationDefault);
}

std::string TW::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    const auto* hrp = stringForHRP(TW::hrp(coin));

    // dispatch
    auto* dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->deriveAddress(coin, derivation, publicKey, p2pkh, hrp);
}

Data TW::addressToData(TWCoinType coin, const std::string& address) {
    const auto* dispatcher = coinDispatcher(coin);
    assert(dispatcher != nullptr);
    return dispatcher->addressToData(coin, address);
}

void TW::anyCoinSign(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->sign(coinType, dataIn, dataOut);
}

std::string TW::anySignJSON(TWCoinType coinType, const std::string& json, const Data& key) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->signJSON(coinType, json, key);
}

bool TW::supportsJSONSigning(TWCoinType coinType) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->supportsJSONSigning();
}

void TW::anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->plan(coinType, dataIn, dataOut);
}

Data TW::anyCoinPreImageHashes(TWCoinType coinType, const Data& txInputData) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->preImageHashes(coinType, txInputData);
}

void TW::anyCoinCompileWithSignatures(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& txOutputOut) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->compile(coinType, txInputData, signatures, publicKeys, txOutputOut);
}

Data TW::anyCoinBuildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId) {
    auto* dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    return dispatcher->buildTransactionInput(coinType, from, to, amount, asset, memo, chainId);
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
    return getCoinInfo(coin).defaultDerivation().xpubVersion;
}

TWHDVersion TW::xprvVersion(TWCoinType coin) {
    return getCoinInfo(coin).defaultDerivation().xprvVersion;
}

TWHDVersion TW::xpubVersionDerivation(TWCoinType coin, TWDerivation derivation) {
    return getCoinInfo(coin).derivationByName(derivation).xpubVersion;
}

TWHDVersion TW::xprvVersionDerivation(TWCoinType coin, TWDerivation derivation) {
    return getCoinInfo(coin).derivationByName(derivation).xprvVersion;
}

DerivationPath TW::derivationPath(TWCoinType coin) {
    return DerivationPath(getCoinInfo(coin).defaultDerivation().path);
}

DerivationPath TW::derivationPath(TWCoinType coin, TWDerivation derivation) {
    return DerivationPath(getCoinInfo(coin).derivationByName(derivation).path);
}

const char* TW::derivationName(TWCoinType coin, TWDerivation derivation) {
    return getCoinInfo(coin).derivationByName(derivation).nameString;
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

const char* TW::chainId(TWCoinType coin) {
    return getCoinInfo(coin).chainId;
}

Hash::Hasher TW::publicKeyHasher(TWCoinType coin) {
    return getCoinInfo(coin).publicKeyHasher;
}

Hash::Hasher TW::base58Hasher(TWCoinType coin) {
    return getCoinInfo(coin).base58Hasher;
}

Hash::Hasher TW::addressHasher(TWCoinType coin) {
    return getCoinInfo(coin).addressHasher;
}

uint32_t TW::slip44Id(TWCoinType coin) {
    return getCoinInfo(coin).slip44;
}

TWString* _Nullable TWCoinTypeConfigurationGetSymbol(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).symbol);
}

int TWCoinTypeConfigurationGetDecimals(enum TWCoinType coin) {
    return getCoinInfo(coin).decimals;
}

TWString* _Nullable TWCoinTypeConfigurationGetTransactionURL(enum TWCoinType coin, TWString* _Nonnull transactionID) {
    std::string txId = TWStringUTF8Bytes(transactionID);
    std::string url = getCoinInfo(coin).explorerTransactionUrl + txId;
    return TWStringCreateWithUTF8Bytes(url.c_str());
}

TWString* _Nullable TWCoinTypeConfigurationGetAccountURL(enum TWCoinType coin, TWString* _Nonnull accountID) {
    std::string accId = TWStringUTF8Bytes(accountID);
    std::string url = getCoinInfo(coin).explorerAccountUrl + accId;
    return TWStringCreateWithUTF8Bytes(url.c_str());
}

TWString* _Nonnull TWCoinTypeConfigurationGetID(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).id);
}

TWString* _Nonnull TWCoinTypeConfigurationGetName(enum TWCoinType coin) {
    return TWStringCreateWithUTF8Bytes(getCoinInfo(coin).name);
}
