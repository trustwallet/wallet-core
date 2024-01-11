// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Ethereum {

/// Entry point for Ethereum and Ethereum-fork coins.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry : public Rust::RustCoinEntryWithSignJSON {
public:
    bool supportsJSONSigning() const final { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const final;
};

} // namespace TW::Ethereum
