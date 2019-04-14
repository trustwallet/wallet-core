// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace TW {

/// Encodes a 16-bit little-endian value into the provided buffer.
inline void encode16LE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>(val >> 8));
}

/// Decodes a 16-bit little-endian value from the provided buffer.
inline uint16_t decode16LE(const uint8_t* _Nonnull src) {
    return static_cast<uint16_t>((src[0]) | ((uint16_t)(src[1]) << 8));
}

/// Encodes a 32-bit little-endian value into the provided buffer.
inline void encode32LE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
}

/// Decodes a 32-bit little-endian value from the provided buffer.
inline uint32_t decode32LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[0])
        | (static_cast<uint32_t>(src[1]) << 8)
        | (static_cast<uint32_t>(src[2]) << 16)
        | (static_cast<uint32_t>(src[3]) << 24);
    // clang-format on
}

/// Encodes a 64-bit little-endian value into the provided buffer.
inline void encode64LE(uint64_t val, std::vector<uint8_t>& data) {
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
inline uint64_t decode64LE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[0])
        | (static_cast<uint64_t>(src[1]) << 8)
        | (static_cast<uint64_t>(src[2]) << 16)
        | (static_cast<uint64_t>(src[3]) << 24)
        | (static_cast<uint64_t>(src[4]) << 32)
        | (static_cast<uint64_t>(src[5]) << 40)
        | (static_cast<uint64_t>(src[6]) << 48)
        | (static_cast<uint64_t>(src[7]) << 56);
    // clang-format on
}

/// Returns the number of bytes it would take to serialize the provided value
/// as a variable-length integer (varint).
inline std::size_t varIntSize(std::size_t value) {
    // The value is small enough to be represented by itself.
    if (value < 0xfd) {
        return 1;
    }

    // Discriminant 1 byte plus 2 bytes for the uint16.
    if (value <= UINT16_MAX) {
        return 3;
    }

    // Discriminant 1 byte plus 4 bytes for the uint32.
    if (value <= UINT32_MAX) {
        return 5;
    }

    // Discriminant 1 byte plus 8 bytes for the uint64.
    return 9;
}

/// Encodes a value as a variable-length integer.
///
/// A variable-length integer (varint) is an encoding for integers up to a max
/// value of 2^64-1 that uses a variable number of bytes depending on the value
/// being encoded. It produces fewer bytes for smaller numbers as opposed to a
/// fixed-size encoding.
///
/// @returns the number of bytes written.
inline std::size_t encodeVarInt(std::size_t size, std::vector<uint8_t>& data) {
    if (size < 0xfd) {
        data.push_back(static_cast<uint8_t>(size));
        return 1;
    }

    if (size <= UINT16_MAX) {
        data.push_back(0xfd);
        encode16LE((uint16_t)size, data);
        return 3;
    }

    if (size <= UINT32_MAX) {
        data.push_back(0xfe);
        encode32LE((uint32_t)size, data);
        return 5;
    }

    data.push_back(0xff);
    encode64LE((uint64_t)size, data);
    return 9;
}

/// Encodes a 16-bit big-endian value into the provided buffer.
inline void encode16BE(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val >> 8));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 16-bit big-endian value from the provided buffer.
inline uint16_t decode16BE(const uint8_t* _Nonnull src) {
    return static_cast<uint16_t>((src[1]) | ((uint16_t)(src[0]) << 8));
}

/// Encodes a 32-bit big-endian value into the provided buffer.
inline void encode32BE(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>(val));
}

/// Decodes a 32-bit big-endian value from the provided buffer.
inline uint32_t decode32BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint32_t>(src[3])
        | (static_cast<uint32_t>(src[2]) << 8)
        | (static_cast<uint32_t>(src[1]) << 16)
        | (static_cast<uint32_t>(src[0]) << 24);
    // clang-format on
}

/// Encodes a 64-bit big-endian value into the provided buffer.
inline void encode64BE(uint64_t val, std::vector<uint8_t>& data) {
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
inline uint64_t decode64BE(const uint8_t* _Nonnull src) {
    // clang-format off
    return static_cast<uint64_t>(src[7])
        | (static_cast<uint64_t>(src[6]) << 8)
        | (static_cast<uint64_t>(src[5]) << 16)
        | (static_cast<uint64_t>(src[4]) << 24)
        | (static_cast<uint64_t>(src[3]) << 32)
        | (static_cast<uint64_t>(src[2]) << 40)
        | (static_cast<uint64_t>(src[1]) << 48)
        | (static_cast<uint64_t>(src[0]) << 56);
    // clang-format on
}

} // namespace TW
