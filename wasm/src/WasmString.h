// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#pragma once

#include <TrustWalletCore/TWString.h>
#include <emscripten/val.h>
#include <string>

namespace TW::Wasm {

/// Converts a TWString * to a JS string val.
/// Zeroes the WASM-heap buffer via TWStringDelete (which calls memzero internally),
/// and explicitly zeroes the intermediate JS-heap Uint8Array before returning.
auto TWStringToVal(TWString *_Nonnull string) -> emscripten::val;

} // namespace TW::Wasm
