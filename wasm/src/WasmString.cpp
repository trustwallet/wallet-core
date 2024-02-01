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
    auto* s = reinterpret_cast<const std::string*>(string);
    auto result = *s;
    return result;
}

} // namespace TW::Wasm
