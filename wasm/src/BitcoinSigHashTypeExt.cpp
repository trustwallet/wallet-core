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
            .class_function("isSingle", &BitcoinSigHashTypeExt::isSingle, allow_raw_pointers())
            .class_function("isNone", &BitcoinSigHashTypeExt::isNone, allow_raw_pointers());
    };
}
