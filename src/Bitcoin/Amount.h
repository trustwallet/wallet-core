// Copyright © 2009-2010 Satoshi Nakamoto
// Copyright © 2009-2016 The Bitcoin Core developers
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace TW::Bitcoin {

/// Amount in satoshis (can be negative)
using Amount = uint64_t;

/// Throws std::invalid_argument if the amount is negative.
/// Use this to validate amounts parsed from Proto::SigningInput.
inline Amount tryToUnsigned(const int64_t amount) {
    if (amount < 0) {
        throw std::invalid_argument("amount cannot be negative: " + std::to_string(amount));
    }
    return static_cast<uint64_t>(amount);
}

inline int64_t tryToSigned(const Amount amount) {
    if (amount > static_cast<uint64_t>(std::numeric_limits<int64_t>::max())) {
        throw std::invalid_argument("amount is too large to fit in int64: " + std::to_string(amount));
    }
    return static_cast<int64_t>(amount);
}

/// uint64_t::MAX (equivalent to -1) is used when signing a transaction with SIGHASH_SINGLE and the output index is out of bounds.
inline Amount sighashSingleAmount() {
    return std::numeric_limits<Amount>::max();
}

} // namespace TW::Bitcoin
