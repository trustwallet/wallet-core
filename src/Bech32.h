// Copyright © 2017 Pieter Wuille
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

namespace TW::Bech32 {

enum ChecksumVariant {
    None = 0,
    Bech32,
    Bech32M,
};

/// Encodes a Bech32 string.
///
/// \returns the encoded string, or an empty string in case of failure.
std::string encode(const std::string& hrp, const Data& values, ChecksumVariant variant);

/// Decodes a Bech32 string.
///
/// \returns a tuple with
/// - the human-readable part
/// - data, or a pair or
/// - ChecksumVariant used
/// or empty values on failure.
std::tuple<std::string, Data, ChecksumVariant> decode(const std::string& str);

/// Converts from one power-of-2 number base to another.
template <int frombits, int tobits, bool pad>
inline bool convertBits(Data& out, const Data& in) {
    int acc = 0;
    int bits = 0;
    const int maxv = (1 << tobits) - 1;
    const int max_acc = (1 << (frombits + tobits - 1)) - 1;
    for (const auto& value : in) {
        acc = ((acc << frombits) | value) & max_acc;
        bits += frombits;
        while (bits >= tobits) {
            bits -= tobits;
            out.push_back((acc >> bits) & maxv);
        }
    }
    if (pad) {
        if (bits)
            out.push_back((acc << (tobits - bits)) & maxv);
    } else if (bits >= frombits || ((acc << (tobits - bits)) & maxv)) {
        return false;
    }
    return true;
}

} // namespace TW::Bech32
