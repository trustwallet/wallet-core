// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmString.h"
#include "Defer.h"

#include <emscripten/val.h>

using namespace emscripten;

namespace TW::Wasm {

auto TWStringToVal(TWString *_Nonnull string) -> val {
    defer {
        TWStringDelete(string);
    };
    auto* s = reinterpret_cast<const std::string*>(string);
    // View the original bytes in-place — no WASM-heap copy.
    auto view = val(typed_memory_view(s->size(), reinterpret_cast<const uint8_t*>(s->data())));
    // Copy once into a JS-heap Uint8Array (not in WASM linear memory).
    auto jsArr = val::global("Uint8Array").new_(view);
    // Decode to a JS string; the original WASM buffer is zeroed by the defer.
    auto result = val::global("TextDecoder").new_().call<val>("decode", jsArr);
    // Zero the intermediate JS-heap byte copy as well.
    jsArr.call<void>("fill", val(0));
    return result;
}

} // namespace TW::Wasm
