// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Greenfield {

/// Entry point for implementation of Greenfield coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
struct Entry final : public Rust::RustCoinEntry {
};

} // namespace TW::Greenfield
