// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include "Coin.h"
#include <emscripten/bind.h>

using namespace emscripten;
using namespace TW;

std::string deriveAddressPriv(TWCoinType coin, const PrivateKey& privateKey) {
    return TW::deriveAddress(coin, privateKey);
}

std::string deriveAddressPub(TWCoinType coin, const PublicKey& publicKey) {
    return TW::deriveAddress(coin, publicKey);
}

EMSCRIPTEN_BINDINGS(wallet_coin_module) {
    function("getCoinTypes", &TW::getCoinTypes);
    function("validateAddress", &TW::validateAddress);
    function("normalizeAddress", &TW::normalizeAddress);
    function("blockchain", &TW::blockchain);
    function("purpose", &TW::purpose);
    function("curve", &TW::curve);
    function("xpubVersion", &TW::xpubVersion);
    function("xprvVersion", &TW::xprvVersion);
    function("derivationPath", &TW::derivationPath);
    function("publicKeyType", &TW::publicKeyType);

    function("deriveAddressPriv", &deriveAddressPriv);
    function("deriveAddressPub", &deriveAddressPub);

    function("publicKeyHasher", &TW::publicKeyHasher);
    function("base58Hasher", &TW::base58Hasher);
    function("staticPrefix", &TW::staticPrefix);
    function("p2pkhPrefix", &TW::p2pkhPrefix);
    function("p2shPrefix", &TW::p2shPrefix);
    function("hrp", &TW::hrp);
    function("anyCoinSign", &TW::anyCoinSign);
    function("slip44Id", &TW::slip44Id);
    function("anySignJSON", &TW::anySignJSON);
    function("supportsJSONSigning", &TW::supportsJSONSigning);
    function("anyCoinPlan", &TW::anyCoinPlan);
    function("getSimilarCoinTypes", &TW::getSimilarCoinTypes);

    function("getPublicKeyType", &TW::publicKeyType);
}
