// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
