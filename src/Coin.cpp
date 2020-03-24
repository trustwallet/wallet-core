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
#include <set>

// Includes for entry points for coin implementations
#include "Aeternity/Entry.h"
#include "Aion/Entry.h"
#include "Algorand/Entry.h"
#include "Bitcoin/Entry.h"
#include "Binance/Entry.h"
#include "Cardano/Entry.h"
#include "Cosmos/Entry.h"
#include "Decred/Entry.h"
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

// Map with coin entry dispatchers, key is coin type
map<TWCoinType, CoinEntry*> dispatchMap = {}; 
// List of supported coint types
set<TWCoinType> coinTypes = {};

void setupDispatchers() {
    std::vector<CoinEntry*> dispatchers = {
        new Aeternity::Entry(),
        new Aion::Entry(),
        new Algorand::Entry(),
        new Binance::Entry(),
        new Bitcoin::Entry(),
        new Cardano::Entry(),
        new Cosmos::Entry(),
        new EOS::Entry(),
        new Ethereum::Entry(),
        new Decred::Entry(),
        new Filecoin::Entry(),
        new FIO::Entry(),
        new Groestlcoin::Entry(),
        new Harmony::Entry(),
        new Icon::Entry(),
        new IoTeX::Entry(),
        new Kusama::Entry(),
        new Nano::Entry(),
        new NEAR::Entry(),
        new Nebulas::Entry(),
        new NEO::Entry(),
        new Nimiq::Entry(),
        new NULS::Entry(),
        new Ontology::Entry(),
        new Polkadot::Entry(),
        new Ripple::Entry(),
        new Solana::Entry(),
        new Stellar::Entry(),
        new Tezos::Entry(),
        new Theta::Entry(),
        new TON::Entry(),
        new Tron::Entry(),
        new VeChain::Entry(),
        new Wanchain::Entry(),
        new Waves::Entry(),
        new Zcash::Entry(),
        new Zilliqa::Entry(),
    }; // end_of_coin_entries_marker_do_not_modify

    dispatchMap.clear();
    coinTypes.clear();
    for (auto d : dispatchers) {
        auto dispCoins = d->coinTypes();
        for (auto c : dispCoins) {
            assert(dispatchMap.find(c) == dispatchMap.end()); // each coin must appear only once
            dispatchMap[c] = d;
            auto setResult = coinTypes.emplace(c);
            assert(setResult.second == true); // each coin must appear only once
        }
    }
    return;
    // Note: dispatchers are created at first use, and never freed
}

inline void setupDispatchersIfNeeded() {
    if (dispatchMap.size() == 0) {
        setupDispatchers();
    }
}

CoinEntry* coinDispatcher(TWCoinType coinType) {
    setupDispatchersIfNeeded();
    // Coin must be present, and not null.  Otherwise that is a fatal code configuration error.
    assert(dispatchMap.find(coinType) != dispatchMap.end()); // coin must be present
    assert(dispatchMap[coinType] != nullptr);
    return dispatchMap[coinType];
}

set<TWCoinType> TW::getCoinTypes() {
    setupDispatchersIfNeeded();
    return coinTypes;
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

std::string TW::normalizeAddress(TWCoinType coin, const std::string &address) {
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

void TW::anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->plan(coinType, dataIn, dataOut);
}

// Coin info accessors

extern const CoinInfo& getCoinInfo(TWCoinType coin); // in generated CoinInfoData.cpp file

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
