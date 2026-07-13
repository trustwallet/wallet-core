// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmData.h"

#include <TrezorCrypto/memzero.h>

using namespace emscripten;

namespace TW::Wasm {

auto DataToVal(Data&& data) -> val {
    auto view = val(typed_memory_view(data.size(), data.data()));
    auto jsArray = val::global("Uint8Array").new_(data.size());
    jsArray.call<void>("set", view);
    if (!data.empty()) { memzero(data.data(), data.size()); }
    return jsArray;
}

auto TWDataToVal(TWData* _Nonnull data) -> val {
    auto* v = reinterpret_cast<Data*>(data);
    auto result = DataToVal(std::move(*v));
    // v is now moved-from (empty); TWDataDelete skips memzero but frees the object.
    TWDataDelete(data);
    return result;
}

} // namespace TW::Wasm
