// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>

#define CAST32(x)    ((uint32_t)(x))
#define CAST64(x)    ((uint64_t)(x))

namespace TW {
namespace Bitcoin {

/// Encodes a 16-bit value into the provided buffer.
static inline void encode16(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>(val >> 8));
}

/// Decodes a 16-bit value from the provided buffer.
static inline uint16_t decode16(const uint8_t *_Nonnull src) {
    return (uint16_t)(src[0]) | ((uint16_t)(src[1]) << 8);
}

/// Encodes a 32-bit value into the provided buffer.
static inline void encode32(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
}

/// Decodes a 32-bit value from the provided buffer.
static inline uint32_t decode32(const uint8_t *_Nonnull src) {
    return CAST32(src[0])
        | (CAST32(src[1]) << 8)
        | (CAST32(src[2]) << 16)
        | (CAST32(src[3]) << 24);
}

/// Encodes a 64-bit value into the provided buffer.
static inline void encode64(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 56)));
}

/// Decodes a 64-bit value from the provided buffer.
static inline uint64_t decode64(const uint8_t *_Nonnull src) {
    return CAST64(src[0])
        | (CAST64(src[1]) << 8)
        | (CAST64(src[2]) << 16)
        | (CAST64(src[3]) << 24)
        | (CAST64(src[4]) << 32)
        | (CAST64(src[5]) << 40)
        | (CAST64(src[6]) << 48)
        | (CAST64(src[7]) << 56);
}

/// Encodes a size into the provided buffer using Bitcoin's compact representation.
inline void writeCompactSize(uint64_t size, std::vector<uint8_t>& data) {
    if (size < 253) {
        data.push_back(static_cast<uint8_t>(size));
        return;
    }
    
    if (size <= UINT16_MAX) {
        data.push_back(253);
        encode16((uint16_t) size, data);
        return;
    }
    
    if (size <= UINT32_MAX) {
        data.push_back(254);
        encode32((uint32_t) size, data);
        return;
    }

    data.push_back(255);
    encode64((uint64_t) size, data);
}

}} // namespace
