// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
