// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <limits>

#include "../BinaryCoding.h"

namespace TW::Waves {

/// Encodes a variable length bytes.
inline void encodeDynamicLengthBytes(const std::vector<uint8_t>& bytes, std::vector<uint8_t>& data) {
    constexpr auto limit = static_cast<size_t>(std::numeric_limits<uint16_t>::max());
    if (bytes.size() > limit) {
        throw std::invalid_argument("Data size exceeds maximum allowed length");
    }
    encode16BE(static_cast<uint16_t>(bytes.size()), data);
    data.insert(data.end(), bytes.begin(), bytes.end());
}

} // namespace TW::Waves
