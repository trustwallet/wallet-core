// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include <emscripten/bind.h>

using namespace emscripten;

#include "BitcoinSigHashTypeExt.h"

namespace TW::Wasm {

    auto BitcoinSigHashTypeExt::isSingle(TWBitcoinSigHashType type) {
        return TWBitcoinSigHashTypeIsSingle(type);
    }
    auto BitcoinSigHashTypeExt::isNone(TWBitcoinSigHashType type) {
        return TWBitcoinSigHashTypeIsNone(type);
    }

    EMSCRIPTEN_BINDINGS(Wasm_BitcoinSigHashTypeExt) {
        class_<BitcoinSigHashTypeExt>("BitcoinSigHashTypeExt")
            .class_function("isSingle", &BitcoinSigHashTypeExt::isSingle)
            .class_function("isNone", &BitcoinSigHashTypeExt::isNone);
    };
}
