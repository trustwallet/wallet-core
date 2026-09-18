// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmData.h"
#include "Defer.h"

#include <TrezorCrypto/memzero.h>

using namespace emscripten;

namespace TW::Wasm {

auto DataToVal(Data&& data) -> val {
    Data local = std::move(data);  // take ownership; caller's object is now empty
    defer {
        if (!local.empty()) { memzero(local.data(), local.size()); }
    };
    auto view = val(typed_memory_view(local.size(), local.data()));
    auto jsArray = val::global("Uint8Array").new_(local.size());
    jsArray.call<void>("set", view);
    return jsArray;
}

auto TWDataToVal(TWData* _Nonnull data) -> val {
    defer {
        TWDataDelete(data);
    };
    auto* v = const_cast<Data*>(reinterpret_cast<const Data*>(data));
    Data local = std::move(*v);  // *v is now genuinely empty; local owns the buffer
    return DataToVal(std::move(local));
}

} // namespace TW::Wasm
