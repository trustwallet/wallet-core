// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "CoinEntry.h"

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

//#include "Aeternity/Address.h"
//#include "Aion/Address.h"
//#include "Algorand/Address.h"
//#include "Bitcoin/Address.h"
//#include "Bitcoin/CashAddress.h"
//#include "Bitcoin/SegwitAddress.h"
//#include "Cardano/AddressV3.h"
//#include "Cosmos/Address.h"
//#include "Decred/Address.h"
//#include "EOS/Address.h"
//#include "Ethereum/Address.h"
//#include "FIO/Address.h"
//#include "Filecoin/Address.h"
//#include "Groestlcoin/Address.h"
//#include "Harmony/Address.h"
//#include "Icon/Address.h"
//#include "IoTeX/Address.h"
//#include "Kusama/Address.h"
//#include "NEAR/Address.h"
//#include "NULS/Address.h"
//#include "Nano/Address.h"
//#include "Nebulas/Address.h"
//#include "Nimiq/Address.h"
//#include "Ontology/Address.h"
//#include "Polkadot/Address.h"
//#include "Ripple/Address.h"
//#include "Ripple/XAddress.h"
//#include "Solana/Address.h"
//#include "Stellar/Address.h"
//#include "TON/Address.h"
//#include "Tezos/Address.h"
//#include "Tron/Address.h"
//#include "Wanchain/Address.h"
//#include "Waves/Address.h"
//#include "Zcash/TAddress.h"
//#include "Zilliqa/Address.h"
//#include "NEO/Address.h"

#include <TrustWalletCore/TWHRP.h>

#include <map>
#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;
using namespace std;

map<TWCoinType, CoinEntry*> dispatchMap; 

int setupDispatchers() {
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
    };
    for (auto d : dispatchers) {
        auto coinTypes = d->coinTypes();
        for (auto c : coinTypes) {
            assert(dispatchMap.find(c) == dispatchMap.end()); // each coin must appear only once
            dispatchMap[c] = d;
        }
    }
    return 0;
    // Note: dispatchers are created from a static initializer, and never freed
}

static int dummy = setupDispatchers();

CoinEntry* coinDispatcher(TWCoinType coinType) {
    // Coin must be present, and not null.  Otherwise that is a fatal code configuration error.
    assert(dispatchMap.find(coinType) != dispatchMap.end()); // coin must be present
    assert(dispatchMap[coinType] != nullptr);
    return dispatchMap[coinType];
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

void TW::anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    assert(dispatcher != nullptr);
    dispatcher->plan(coinType, dataIn, dataOut);
}

#pragma clang diagnostic pop
