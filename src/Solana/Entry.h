// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "PublicKey.h"
#include "rust/RustCoinEntry.h"

namespace TW::Solana {

/// Entry point for implementation of Solana coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry final : public Rust::RustCoinEntryWithSignJSON {
public:
    bool supportsJSONSigning() const override { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const override;
    PrivateKey decodePrivateKey(TWCoinType coin, const std::string& privateKey) const override;
};

} // namespace TW::Solana
