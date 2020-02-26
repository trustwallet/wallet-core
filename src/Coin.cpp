// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"

#include "CoinEntry.h"

#include "Bitcoin/Entry.h"
#include "Binance/Entry.h"
#include "Ethereum/Entry.h"
#include "Groestlcoin/Entry.h"

#include "Aeternity/Address.h"
#include "Aion/Address.h"
#include "Algorand/Address.h"
//#include "Bitcoin/Address.h"
//#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Cardano/AddressV3.h"
#include "Cosmos/Address.h"
#include "Decred/Address.h"
#include "EOS/Address.h"
#include "Ethereum/Address.h"
#include "FIO/Address.h"
#include "Filecoin/Address.h"
//#include "Groestlcoin/Address.h"
#include "Harmony/Address.h"
#include "Icon/Address.h"
#include "IoTeX/Address.h"
#include "Kusama/Address.h"
#include "NEAR/Address.h"
#include "NULS/Address.h"
#include "Nano/Address.h"
#include "Nebulas/Address.h"
#include "Nimiq/Address.h"
#include "Ontology/Address.h"
#include "Polkadot/Address.h"
#include "Ripple/Address.h"
#include "Ripple/XAddress.h"
#include "Solana/Address.h"
#include "Stellar/Address.h"
#include "TON/Address.h"
#include "Tezos/Address.h"
#include "Tron/Address.h"
#include "Wanchain/Address.h"
#include "Waves/Address.h"
#include "Zcash/TAddress.h"
#include "Zilliqa/Address.h"
#include "NEO/Address.h"

#include <TrustWalletCore/TWHRP.h>

#include <map>
#pragma clang diagnostic push
#pragma clang diagnostic fatal "-Wswitch"

using namespace TW;
using namespace std;

map<TWCoinType, CoinEntry*> dispatchMap; 

int setupDispatchers() {
    std::vector<CoinEntry*> dispatchers = {
        new Binance::Entry(),
        new Bitcoin::Entry(),
        new Ethereum::Entry(),
        new Groestlcoin::Entry(),
    };
    for (auto d : dispatchers) {
        auto coinTypes = d->coinTypes();
        for (auto c : coinTypes) {
            dispatchMap[c] = d;
        }
    }
    return 0;
    // TODO: delete at the end?
}

static int dummy = setupDispatchers();

CoinEntry* coinDispatcher(TWCoinType coinType) {
    if (dispatchMap.find(coinType) == dispatchMap.end()) {
        // no such dispatcher
        return nullptr;
    }
    assert(dispatchMap[coinType] != nullptr);
    return dispatchMap[coinType];
}

bool TW::validateAddress(TWCoinType coin, const std::string& string) {
    auto p2pkh = TW::p2pkhPrefix(coin);
    auto p2sh = TW::p2shPrefix(coin);
    auto hrp = stringForHRP(TW::hrp(coin));

    // dispatch
    auto dispatcher = coinDispatcher(coin);
    if (dispatcher != nullptr) {
        return dispatcher->validateAddress(coin, string, p2pkh, p2sh, hrp);
    }

    // TODO: remove the switch once all coins have dispatchers
    switch (coin) {
    case TWCoinTypeAeternity:
        return Aeternity::Address::isValid(string);

    case TWCoinTypeAion:
        return Aion::Address::isValid(string);

    //case TWCoinTypeBinance:
    //    return Binance::Address::isValid(string);

    case TWCoinTypeCosmos:
    case TWCoinTypeKava:
    case TWCoinTypeTerra:
        return Cosmos::Address::isValid(string, hrp);

    //case TWCoinTypeBitcoin:
    //case TWCoinTypeDigiByte:
    //case TWCoinTypeLitecoin:
    //case TWCoinTypeMonacoin:
    //case TWCoinTypeQtum:
    //case TWCoinTypeViacoin:
    //    return Bitcoin::SegwitAddress::isValid(string, hrp) ||
    //           Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    //case TWCoinTypeBitcoinCash:
    //    return Bitcoin::CashAddress::isValid(string) ||
    //           Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    //case TWCoinTypeDash:
    //case TWCoinTypeDogecoin:
    //case TWCoinTypeRavencoin:
    //case TWCoinTypeZcoin:
    //    return Bitcoin::Address::isValid(string, {{p2pkh}, {p2sh}});

    case TWCoinTypeDecred:
        return Decred::Address::isValid(string);

    //case TWCoinTypeGroestlcoin:
    //    return Bitcoin::SegwitAddress::isValid(string, hrp) ||
    //           Groestlcoin::Address::isValid(string, {p2pkh, p2sh});

    case TWCoinTypeCallisto:
    //case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeTheta:
        return Ethereum::Address::isValid(string);

    case TWCoinTypeEOS:
        return EOS::Address::isValid(string);

    case TWCoinTypeFIO:
        return FIO::Address::isValid(string);

    case TWCoinTypeWanchain:
        return Wanchain::Address::isValid(string);

    case TWCoinTypeICON:
        return Icon::Address::isValid(string);

    case TWCoinTypeIoTeX:
        return IoTeX::Address::isValid(string);

    case TWCoinTypeOntology:
        return Ontology::Address::isValid(string);

    case TWCoinTypeNimiq:
        return Nimiq::Address::isValid(string);

    case TWCoinTypeXRP:
        return Ripple::Address::isValid(string) || Ripple::XAddress::isValid(string);

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address::isValid(string);

    case TWCoinTypeTezos:
        return Tezos::Address::isValid(string);

    case TWCoinTypeTron:
        return Tron::Address::isValid(string);

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress::isValid(string);

    case TWCoinTypeZilliqa:
        return Zilliqa::Address::isValid(string);

    case TWCoinTypeNano:
        return Nano::Address::isValid(string);

    case TWCoinTypeNEAR:
        return NEAR::Address::isValid(string);

    case TWCoinTypeNULS:
        return NULS::Address::isValid(string);

    case TWCoinTypeWaves:
        return Waves::Address::isValid(string);

    case TWCoinTypeNebulas:
        return Nebulas::Address::isValid(string);

    case TWCoinTypeHarmony:
        return Harmony::Address::isValid(string);

    case TWCoinTypeSolana:
        return Solana::Address::isValid(string);

    case TWCoinTypeTON:
        return TON::Address::isValid(string);

    case TWCoinTypeAlgorand:
        return Algorand::Address::isValid(string);

    case TWCoinTypeKusama:
        return Kusama::Address::isValid(string);

    case TWCoinTypePolkadot:
        return Polkadot::Address::isValid(string);

    case TWCoinTypeCardano:
        return Cardano::AddressV3::isValid(string);

    case TWCoinTypeNEO:
        return NEO::Address::isValid(string);

    case TWCoinTypeFilecoin:
        return Filecoin::Address::isValid(string);

    default:
        assert(false);
        return false;
    }
}

std::string TW::normalizeAddress(TWCoinType coin, const std::string &address) {
    if (!TW::validateAddress(coin, address)) {
        return "";
    }

    // dispatch
    auto dispatcher = coinDispatcher(coin);
    if (dispatcher != nullptr) {
        return dispatcher->normalizeAddress(coin, address);
    }

    // TODO: remove the switch once all coins have dispatchers
    switch (coin) {
    //case TWCoinTypeBitcoinCash:
    //    // normalized with bitcoincash: prefix
    //    if (Bitcoin::CashAddress::isValid(address)) {
    //        return Bitcoin::CashAddress(address).string();
    //    } else {
    //        return std::string(address);
    //    }
    case TWCoinTypeCallisto:
    //case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeTheta:
        // normalized with EIP55 checksum
        return Ethereum::Address(address).string();
    case TWCoinTypeWanchain:
        // normalized with Wanchain checksum
        return Wanchain::Address(address).string();

    default:
        return std::string(address);
    }
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
    if (dispatcher != nullptr) {
        return dispatcher->deriveAddress(coin, publicKey, p2pkh, hrp);
    }

    // TODO: remove the switch once all coins have dispatchers
    switch (coin) {
    case TWCoinTypeAeternity:
        return Aeternity::Address(publicKey).string();

    //case TWCoinTypeBinance:
    //    return Binance::Address(publicKey).string();

    //case TWCoinTypeBitcoin:
    //case TWCoinTypeDigiByte:
    //case TWCoinTypeGroestlcoin:
    //case TWCoinTypeLitecoin:
    //case TWCoinTypeViacoin:
        return Bitcoin::SegwitAddress(publicKey, 0, hrp).string();

    //case TWCoinTypeBitcoinCash:
    //    return Bitcoin::CashAddress(publicKey).string();

    case TWCoinTypeCosmos:
    case TWCoinTypeKava:
    case TWCoinTypeTerra:
        return Cosmos::Address(hrp, publicKey).string();

    //case TWCoinTypeDash:
    //case TWCoinTypeDogecoin:
    //case TWCoinTypeMonacoin:
    //case TWCoinTypeQtum:
    //case TWCoinTypeRavencoin:
    //case TWCoinTypeZcoin:
    //    return Bitcoin::Address(publicKey, p2pkh).string();

    case TWCoinTypeDecred:
        return Decred::Address(publicKey).string();

    case TWCoinTypeCallisto:
    //case TWCoinTypeEthereum:
    case TWCoinTypeEthereumClassic:
    case TWCoinTypeGoChain:
    case TWCoinTypePOANetwork:
    case TWCoinTypeThunderToken:
    case TWCoinTypeTomoChain:
    case TWCoinTypeVeChain:
    case TWCoinTypeTheta:
        return Ethereum::Address(publicKey).string();

    case TWCoinTypeEOS:
        return EOS::Address(publicKey).string();

    case TWCoinTypeFIO:
        return FIO::Address(publicKey).string();

    case TWCoinTypeWanchain:
        return Wanchain::Address(publicKey).string();

    case TWCoinTypeICON:
        return Icon::Address(publicKey, Icon::TypeAddress).string();

    case TWCoinTypeIoTeX:
        return IoTeX::Address(publicKey).string();

    case TWCoinTypeOntology:
        return Ontology::Address(publicKey).string();

    case TWCoinTypeNimiq:
        return Nimiq::Address(publicKey).string();

    case TWCoinTypeAion:
        return Aion::Address(publicKey).string();

    case TWCoinTypeXRP:
        return Ripple::Address(publicKey).string();

    case TWCoinTypeTezos:
        return Tezos::Address(publicKey).string();

    case TWCoinTypeTron:
        return Tron::Address(publicKey).string();

    case TWCoinTypeZelcash:
    case TWCoinTypeZcash:
        return Zcash::TAddress(publicKey, p2pkh).string();

    case TWCoinTypeZilliqa:
        return Zilliqa::Address(publicKey).string();

    case TWCoinTypeStellar:
    case TWCoinTypeKin:
        return Stellar::Address(publicKey).string();

    case TWCoinTypeNano:
        return Nano::Address(publicKey).string();

    case TWCoinTypeNULS:
        return NULS::Address(publicKey).string();

    case TWCoinTypeNEAR:
        return NEAR::Address(publicKey).string();

    case TWCoinTypeWaves:
        return Waves::Address(publicKey).string();

    case TWCoinTypeNebulas:
        return Nebulas::Address(publicKey).string();

    case TWCoinTypeHarmony:
        return Harmony::Address(publicKey).string();

    case TWCoinTypeSolana:
        return Solana::Address(publicKey).string();

    case TWCoinTypeTON:
        return TON::Address(publicKey).string();

    case TWCoinTypeAlgorand:
        return Algorand::Address(publicKey).string();

    case TWCoinTypeKusama:
        return Kusama::Address(publicKey).string();

    case TWCoinTypePolkadot:
        return Polkadot::Address(publicKey).string();

    case TWCoinTypeCardano:
        return Cardano::AddressV3(publicKey).string();

    case TWCoinTypeNEO:
        return NEO::Address(publicKey).string();

    case TWCoinTypeFilecoin:
        return Filecoin::Address(publicKey).string();

    default:
        assert(false);
        return "";
    }
}

bool TW::anyCoinSign(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    if (dispatcher != nullptr) {
        dispatcher->sign(coinType, dataIn, dataOut);
        return true;
    }
    return false;
}

bool TW::anyCoinPlan(TWCoinType coinType, const Data& dataIn, Data& dataOut) {
    auto dispatcher = coinDispatcher(coinType);
    if (dispatcher != nullptr) {
        dispatcher->plan(coinType, dataIn, dataOut);
        return true;
    }
    return false;
}

#pragma clang diagnostic pop
