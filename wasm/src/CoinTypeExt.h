// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "generated/PrivateKey.h"
#include "generated/PublicKey.h"

#include <emscripten/bind.h>

using namespace emscripten;

#include <string>

#include "WasmString.h"
#include "WasmData.h"

namespace TW::Wasm {

    class CoinTypeExt {
    public:
        static auto blockchain(TWCoinType coin);
        static auto purpose(TWCoinType coin);
        static auto curve(TWCoinType coin);
        static auto xpubVersion(TWCoinType coin);
        static auto xprvVersion(TWCoinType coin);
        static auto hrp(TWCoinType coin);
        static auto p2pkhPrefix(TWCoinType coin);
        static auto p2shPrefix(TWCoinType coin);
        static auto staticPrefix(TWCoinType coin);
        static auto chainId(TWCoinType coin);
        static auto slip44Id(TWCoinType coin);
        static auto ss58Prefix(TWCoinType coin);
        static auto publicKeyType(TWCoinType coin);
        static auto validate(TWCoinType coin, const std::string& address);
        static auto derivationPath(TWCoinType coin);
        static auto derivationPathWithDerivation(TWCoinType coin, TWDerivation derivation);
        static auto deriveAddress(TWCoinType coin, WasmPrivateKey* privateKey);
        static auto deriveAddressFromPublicKey(TWCoinType coin, WasmPublicKey* publicKey);
        static auto deriveAddressFromPublicKeyAndDerivation(TWCoinType coin, WasmPublicKey* publicKey, TWDerivation derivation);
    };
}
