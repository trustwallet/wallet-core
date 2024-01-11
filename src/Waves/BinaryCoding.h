// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "../BinaryCoding.h"

namespace TW::Waves {

/// Encodes a variable length bytes.
inline void encodeDynamicLengthBytes(std::vector<uint8_t> bytes, std::vector<uint8_t> &data) {
    encode16BE(static_cast<uint16_t>(bytes.size()), data);
    data.insert(data.end(), bytes.begin(), bytes.end());
}

} // namespace TW::Waves
