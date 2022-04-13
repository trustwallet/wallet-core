// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "WASMData.h"

using namespace emscripten;

namespace TW::Wasm {

auto DataToVal(const Data& data) -> val {
    return val(typed_memory_view(data.size(), data.data()));
}

auto TWDataToVal(TWData *_Nonnull data) -> val {
    auto* v = reinterpret_cast<const Data*>(data);
    // Data result = Data(v->begin(), v->end());
    // TWDataDelete(data);
    return DataToVal(*v);
}

} // namespace TW::Wasm
