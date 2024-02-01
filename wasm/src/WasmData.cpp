// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmData.h"
#include "Defer.h"

using namespace emscripten;

namespace TW::Wasm {

auto DataToVal(Data data) -> val {
    auto view = val(typed_memory_view(data.size(), data.data()));
    auto jsArray = val::global("Uint8Array").new_(data.size());
    jsArray.call<void>("set", view);
    return jsArray;
}

auto TWDataToVal(TWData* _Nonnull data) -> val {
    defer {
        TWDataDelete(data);
    };
    auto* v = reinterpret_cast<const Data*>(data);
    return DataToVal(*v);
}

} // namespace TW::Wasm
