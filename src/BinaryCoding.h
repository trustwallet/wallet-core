// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>
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
void encode64LE(uint64_t val, std::vector<uint8_t>& data);

/// Decodes a 64-bit little-endian value from the provided buffer.
uint64_t decode64LE(const uint8_t* _Nonnull src);

/// Returns the number of bytes it would take to serialize the provided value
/// as a variable-length integer (varint).
uint8_t varIntSize(uint64_t value);

/// Encodes a value as a variable-length integer.
///
/// A variable-length integer (varint) is an encoding for integers up to a max
/// value of 2^64-1 that uses a variable number of bytes depending on the value
/// being encoded. It produces fewer bytes for smaller numbers as opposed to a
/// fixed-size encoding.  Little endian byte order is used.
///
/// \returns the number of bytes written.
uint8_t encodeVarInt(uint64_t size, std::vector<uint8_t>& data);

/// Decodes an integer as a variable-length integer. See encodeVarInt().
///
/// \returns a tuple with a success indicator and the decoded integer.
std::tuple<bool, uint64_t> decodeVarInt(const Data& in, size_t& indexInOut);

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
void encode64BE(uint64_t val, std::vector<uint8_t>& data);

/// Decodes a 64-bit big-endian value from the provided buffer.
uint64_t decode64BE(const uint8_t* _Nonnull src);

/// Encodes an ASCII string prefixed by the length (varInt)
void encodeString(const std::string& str, std::vector<uint8_t>& data);

/// Decodes an ASCII string prefixed by its length (varInt) 
/// \returns a tuple with a success indicator and the decoded string.
std::tuple<bool, std::string> decodeString(const Data& in, size_t& indexInOut);

} // namespace TW
