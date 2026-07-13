// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#pragma once

#include <TrustWalletCore/TWString.h>
#include <emscripten/val.h>
#include <string>

namespace TW::Wasm {

/// Converts a TWString * to a JS string val, zeroing all WASM-heap copies and
/// deleting the TWString * when done.  Prefer this over TWStringToStd for any
/// string that may contain secret material (keys, mnemonics, signatures).
auto TWStringToVal(TWString *_Nonnull string) -> emscripten::val;

} // namespace TW::Wasm
