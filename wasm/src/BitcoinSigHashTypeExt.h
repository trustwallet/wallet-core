// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
