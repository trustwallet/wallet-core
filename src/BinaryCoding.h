// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace TW {

/// Encodes a 16-bit little-endian value into the provided buffer.
static inline void encode16LE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>(val >> 8));
}

/// Decodes a 16-bit little-endian value from the provided buffer.
static inline uint16_t decode16LE(const uint8_t *_Nonnull src) {
    return (uint16_t)(src[0]) | ((uint16_t)(src[1]) << 8);
}

/// Encodes a 32-bit little-endian value into the provided buffer.
static inline void encode32LE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
}

/// Decodes a 32-bit little-endian value from the provided buffer.
static inline uint32_t decode32LE(const uint8_t *_Nonnull src) {
    return static_cast<uint32_t>(src[0])
        | (static_cast<uint32_t>(src[1]) << 8)
        | (static_cast<uint32_t>(src[2]) << 16)
        | (static_cast<uint32_t>(src[3]) << 24);
}

/// Encodes a 64-bit little-endian value into the provided buffer.
static inline void encode64LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 56)));
}

/// Decodes a 64-bit little-endian value from the provided buffer.
static inline uint64_t decode64LE(const uint8_t *_Nonnull src) {
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40)
        | (static_cast<uint64_t>(src[6]) << 48)
        | (static_cast<uint64_t>(src[7]) << 56);
}

/// Encodes a size into the provided buffer using Bitcoin's compact representation.
inline void writeCompactSize(uint64_t size, std::vector<uint8_t>& data) {
    if (size < 253) {
        data.push_back(static_cast<uint8_t>(size));
        return;
    }
    
    if (size <= UINT16_MAX) {
        data.push_back(253);
        encode16LE((uint16_t) size, data);
        return;
    }
    
    if (size <= UINT32_MAX) {
        data.push_back(254);
        encode32LE((uint32_t) size, data);
        return;
    }

    data.push_back(255);
    encode64LE((uint64_t) size, data);
}

/// Encodes a 16-bit big-endian value into the provided buffer.
static inline void encode16BE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val >> 8));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 16-bit big-endian value from the provided buffer.
static inline uint16_t decode16BE(const uint8_t *_Nonnull src) {
    return (uint16_t)(src[1]) | ((uint16_t)(src[0]) << 8);
}

/// Encodes a 32-bit big-endian value into the provided buffer.
static inline void encode32BE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 32-bit big-endian value from the provided buffer.
static inline uint32_t decode32BE(const uint8_t *_Nonnull src) {
    return static_cast<uint32_t>(src[3])
        | (static_cast<uint32_t>(src[2]) << 8)
        | (static_cast<uint32_t>(src[1]) << 16)
        | (static_cast<uint32_t>(src[0]) << 24);
}

/// Encodes a 64-bit big-endian value into the provided buffer.
static inline void encode64BE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 56)));
    data.push_back(static_cast<uint8_t>((val >> 48)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 64-bit big-endian value from the provided buffer.
static inline uint64_t decode64BE(const uint8_t *_Nonnull src) {
    return static_cast<uint64_t>(src[7])
        | (static_cast<uint64_t>(src[6]) << 8)
        | (static_cast<uint64_t>(src[5]) << 16)
        | (static_cast<uint64_t>(src[4]) << 24)
        | (static_cast<uint64_t>(src[3]) << 32)
        | (static_cast<uint64_t>(src[2]) << 40)
        | (static_cast<uint64_t>(src[1]) << 48)
        | (static_cast<uint64_t>(src[0]) << 56);
}

} // namespace
