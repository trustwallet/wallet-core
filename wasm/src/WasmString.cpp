// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#include "WasmString.h"
#include "Defer.h"

namespace TW::Wasm {

auto TWStringToStd(TWString *_Nullable string) -> std::string {
    if (string == nullptr) {
        return {};
    }
    defer {
        TWStringDelete(string);
    };
    return *reinterpret_cast<const std::string*>(string);
}

} // namespace TW::Wasm
