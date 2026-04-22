// Copyright © 2009-2010 Satoshi Nakamoto
// Copyright © 2009-2016 The Bitcoin Core developers
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace TW::Bitcoin {

/// Amount in satoshis (can be negative)
using Amount = int64_t;

/// Throws std::invalid_argument if the amount is negative.
/// Use this to validate amounts parsed from Proto::SigningInput.
inline void assertValidAmount(const Amount amount) {
    if (amount < 0) {
        throw std::invalid_argument("amount cannot be negative: " + std::to_string(amount));
    }
}

} // namespace TW::Bitcoin
