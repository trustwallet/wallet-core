// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
