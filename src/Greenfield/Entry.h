// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Greenfield {

/// Entry point for implementation of Greenfield coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
struct Entry final : public Rust::RustCoinEntry {
};

} // namespace TW::Greenfield
