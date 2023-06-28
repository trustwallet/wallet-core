// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCoinType.h>
#include "WasmString.h"

#include "generated/PrivateKey.h"
#include "generated/PublicKey.h"

#include <emscripten/bind.h>

using namespace emscripten;

#include "CoinTypeExt.h"

namespace TW::Wasm {

    auto CoinTypeExt::blockchain(TWCoinType coin) {
        return TWCoinTypeBlockchain(coin);
    }
    auto CoinTypeExt::purpose(TWCoinType coin) {
        return TWCoinTypePurpose(coin);
    }
    auto CoinTypeExt::curve(TWCoinType coin) {
        return TWCoinTypeCurve(coin);
    }
    auto CoinTypeExt::xpubVersion(TWCoinType coin) {
        return TWCoinTypeXpubVersion(coin);
    }
    auto CoinTypeExt::xprvVersion(TWCoinType coin) {
        return TWCoinTypeXprvVersion(coin);
    }
    auto CoinTypeExt::hrp(TWCoinType coin) {
        return TWCoinTypeHRP(coin);
    }
    auto CoinTypeExt::p2pkhPrefix(TWCoinType coin) {
        return TWCoinTypeP2pkhPrefix(coin);
    }
    auto CoinTypeExt::p2shPrefix(TWCoinType coin) {
        return TWCoinTypeP2shPrefix(coin);
    }
    auto CoinTypeExt::staticPrefix(TWCoinType coin) {
        return TWCoinTypeStaticPrefix(coin);
    }
    auto CoinTypeExt::chainId(TWCoinType coin) {
        return TWStringToStd(TWCoinTypeChainId(coin));
    }
    auto CoinTypeExt::slip44Id(TWCoinType coin) {
        return TWCoinTypeSlip44Id(coin);
    }
    auto CoinTypeExt::ss58Prefix(TWCoinType coin) {
        return TWCoinTypeSS58Prefix(coin);
    }
    auto CoinTypeExt::publicKeyType(TWCoinType coin) {
        return TWCoinTypePublicKeyType(coin);
    }
    auto CoinTypeExt::validate(TWCoinType coin, const std::string& address) {
        return TWCoinTypeValidate(coin, &address);
    }
    auto CoinTypeExt::derivationPath(TWCoinType coin) {
        return TWStringToStd(TWCoinTypeDerivationPath(coin));
    }
    auto CoinTypeExt::derivationPathWithDerivation(TWCoinType coin, TWDerivation derivation) {
        return TWStringToStd(TWCoinTypeDerivationPathWithDerivation(coin, derivation));
    }
    auto CoinTypeExt::deriveAddress(TWCoinType coin, WasmPrivateKey* privateKey) {
        return TWStringToStd(TWCoinTypeDeriveAddress(coin, privateKey->instance));
    }
    auto CoinTypeExt::deriveAddressFromPublicKey(TWCoinType coin, WasmPublicKey* publicKey) {
        return TWStringToStd(TWCoinTypeDeriveAddressFromPublicKey(coin, publicKey->instance));
    }
    auto CoinTypeExt::deriveAddressFromPublicKeyAndDerivation(TWCoinType coin, WasmPublicKey* publicKey, TWDerivation derivation) {
        return TWStringToStd(TWCoinTypeDeriveAddressFromPublicKeyAndDerivation(coin, publicKey->instance, derivation));
    }

    EMSCRIPTEN_BINDINGS(Wasm_CoinTypeExt) {
        class_<CoinTypeExt>("CoinTypeExt")
            .class_function("blockchain", &CoinTypeExt::blockchain)
            .class_function("purpose", &CoinTypeExt::purpose)
            .class_function("curve", &CoinTypeExt::curve)
            .class_function("xpubVersion", &CoinTypeExt::xpubVersion)
            .class_function("xprvVersion", &CoinTypeExt::xprvVersion)
            .class_function("hrp", &CoinTypeExt::hrp)
            .class_function("p2pkhPrefix", &CoinTypeExt::p2pkhPrefix)
            .class_function("p2shPrefix", &CoinTypeExt::p2shPrefix)
            .class_function("staticPrefix", &CoinTypeExt::staticPrefix)
            .class_function("chainId", &CoinTypeExt::chainId)
            .class_function("slip44Id", &CoinTypeExt::slip44Id)
            .class_function("ss58Prefix", &CoinTypeExt::ss58Prefix)
            .class_function("publicKeyType", &CoinTypeExt::publicKeyType)
            .class_function("validate", &CoinTypeExt::validate)
            .class_function("derivationPath", &CoinTypeExt::derivationPath)
            .class_function("derivationPathWithDerivation", &CoinTypeExt::derivationPathWithDerivation)
            .class_function("deriveAddress", &CoinTypeExt::deriveAddress, allow_raw_pointers())
            .class_function("deriveAddressFromPublicKey", &CoinTypeExt::deriveAddressFromPublicKey, allow_raw_pointers())
            .class_function("deriveAddressFromPublicKeyAndDerivation", &CoinTypeExt::deriveAddressFromPublicKeyAndDerivation, allow_raw_pointers());
    };
}
