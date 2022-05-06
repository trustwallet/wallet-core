// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
