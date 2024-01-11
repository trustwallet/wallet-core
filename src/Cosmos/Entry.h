// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Cosmos {

/// Entry point for implementation of Cosmos coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry : public Rust::RustCoinEntryWithSignJSON {
public:
    ~Entry() override = default;
    bool supportsJSONSigning() const final { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const final;
};

} // namespace TW::Cosmos
