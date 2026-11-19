// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmString.h"
#include "Defer.h"

namespace TW::Wasm {

auto TWStringToStd(TWString *_Nonnull string) -> std::string {
    defer {
        TWStringDelete(string);
    };
    return *reinterpret_cast<const std::string*>(string);
}

auto TWStringToVal(TWString *_Nullable string) -> emscripten::val {
    if (string == nullptr) {
        return emscripten::val::null();
    }
    return emscripten::val(TWStringToStd(string));
}

} // namespace TW::Wasm
