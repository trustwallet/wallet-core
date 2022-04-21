// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#include "WasmString.h"
#include "Defer.h"

namespace TW::Wasm {

auto TWStringToStd(TWString *_Nonnull string) -> std::string {
    defer {
        TWStringDelete(string);
    };
    auto* s = reinterpret_cast<const std::string*>(string);
    auto result = *s;
    return result;
}

} // namespace TW::Wasm
