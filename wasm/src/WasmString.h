// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#pragma once

#include <TrustWalletCore/TWString.h>
#include <emscripten/val.h>
#include <string>

namespace TW::Wasm {

/// Converts a TWString * to std::string, deleting the TWString * when done.
auto TWStringToStd(TWString *_Nonnull string) -> std::string;

/// Converts a nullable TWString * to a JS string, deleting the TWString * when done.
/// A null TWString * (returned by `_Nullable` functions on failure) yields JS null.
auto TWStringToVal(TWString *_Nullable string) -> emscripten::val;

} // namespace TW::Wasm
