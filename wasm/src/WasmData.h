// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#pragma once

#include <TrustWalletCore/TWData.h>
#include <emscripten/val.h>

#include "Data.h"

namespace TW::Wasm {

auto DataToVal(Data data) -> emscripten::val;

/// Converts a TWData * to Uint8Array, deleting the TWData * when done.
auto TWDataToVal(TWData *_Nonnull data) -> emscripten::val;

} // namespace TW::Wasm
