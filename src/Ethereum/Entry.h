// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Ethereum {

/// Entry point for Ethereum and Ethereum-fork coins.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry : public Rust::RustCoinEntry {
public:
    bool supportsJSONSigning() const final { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const final;
};

} // namespace TW::Ethereum
