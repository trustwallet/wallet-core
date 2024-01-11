// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Binance {

/// Binance entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Rust::RustCoinEntryWithSignJSON {
public:
    bool supportsJSONSigning() const override { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const override;
};

} // namespace TW::Binance
