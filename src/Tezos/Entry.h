// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Tezos {

class Entry final : public Rust::RustCoinEntryWithSignJSON {
public:
    bool supportsJSONSigning() const final { return true; }
    std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const final;
};

} // namespace TW::Tezos
