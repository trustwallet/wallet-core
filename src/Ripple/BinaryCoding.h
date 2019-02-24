// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace TW {
namespace Ripple {

enum class FieldType;

/// Encodes a 16-bit value into the provided buffer.
static inline void encode16(uint16_t val, std::vector<uint8_t>& data) {
    data.push_back (static_cast<unsigned char> (val >> 8));
    data.push_back (static_cast<unsigned char> (val & 0xff));
}

/// Encodes a 32-bit value into the provided buffer.
static inline void encode32(uint32_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 16) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 8) & 0xff));
    data.push_back(static_cast<uint8_t>(val));
}

/// Encodes a 64-bit value into the provided buffer.
static inline void encode64(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>((val >> 56)));
    data.push_back(static_cast<uint8_t>((val >> 48) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 40) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 32) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 24) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 16) & 0xff));
    data.push_back(static_cast<uint8_t>((val >> 8) & 0xff));
    data.push_back(static_cast<uint8_t>(val & 0xff));
}

/// Encodes a field type.
static inline void encodeType(FieldType type, int key, std::vector<uint8_t>& data) {
    const auto typeValue = static_cast<int>(type);
    if (key <= 0xf) {
        data.push_back(static_cast<uint8_t>((typeValue << 4) | key));
    } else {
        data.push_back(static_cast<uint8_t>(typeValue << 4));
        data.push_back(static_cast<uint8_t>(key));
    }
}

/// Encodes a variable length.
static inline void encodeVariableLength(int length, std::vector<uint8_t>& data) {
    if (length <= 192) {
        data.push_back(static_cast<unsigned char> (length));
    } else if (length <= 12480) {
        length -= 193;
        data.push_back(static_cast<unsigned char> (length >> 8));
        data.push_back(static_cast<unsigned char> (length & 0xff));
    } else if (length <= 918744) {
        length -= 12481;
        data.push_back(static_cast<unsigned char> (length >> 16));
        data.push_back(static_cast<unsigned char> ((length >> 8) & 0xff));
        data.push_back(static_cast<unsigned char> (length & 0xff));
    }
}

/// Encodes a variable length bytes.
static inline void encodeBytes(std::vector<uint8_t> bytes, std::vector<uint8_t>& data) {
    encodeVariableLength(bytes.size(), data);
    data.insert(data.end(), bytes.begin(), bytes.end());
}

}} // namespace
