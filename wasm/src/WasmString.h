// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//

#pragma once

#include <TrustWalletCore/TWString.h>
#include <string>

namespace TW::Wasm {

/// Converts a TWString * to std::string, deleting the TWString * when done.
auto TWStringToStd(TWString *_Nonnull string) -> std::string;

} // namespace TW::Wasm
