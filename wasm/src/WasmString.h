// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//

#pragma once

#include <TrustWalletCore/TWString.h>
#include <string>

namespace TW::Wasm {

/// Converts a TWString * to std::string, deleting the TWString * when done.
auto TWStringToStd(TWString *_Nonnull string) -> std::string;

} // namespace TW::Wasm
