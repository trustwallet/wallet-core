// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include <emscripten/bind.h>

using namespace emscripten;

namespace TW::Wasm {

    class BitcoinSigHashTypeExt {
    public:
        static auto isSingle(TWBitcoinSigHashType type);
        static auto isNone(TWBitcoinSigHashType type);
    };
}
