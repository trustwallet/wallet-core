// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::{BLOCKCHAIN} {

/// Entry point for {BLOCKCHAIN} coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry : public Rust::RustCoinEntry {
};

} // namespace TW::{BLOCKCHAIN}

