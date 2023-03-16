// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include "Data.h"

namespace TW::Ripple {

enum class FieldType;

/// Encodes a field type.
inline void encodeType(FieldType type, int key, std::vector<uint8_t>& data) {
    const auto typeValue = static_cast<int>(type);
    if (typeValue <= 0xf) {
        if (key <= 0xf) {
            data.emplace_back(static_cast<uint8_t>((typeValue << 4) | key));
        } else {
            data.emplace_back(static_cast<uint8_t>(typeValue << 4));
            data.emplace_back(static_cast<uint8_t>(key));
        }
    } else if (key <= 0xf) {
        data.emplace_back(static_cast<uint8_t>(key));
        data.emplace_back(static_cast<uint8_t>(typeValue));
    } else {
        data.emplace_back(0);
        data.emplace_back(static_cast<uint8_t>(typeValue));
        data.emplace_back(static_cast<uint8_t>(key));
    }
}

/// Encodes a variable length.
inline void encodeVariableLength(size_t length, std::vector<uint8_t>& data) {
    if (length <= 192) {
        data.push_back(static_cast<unsigned char>(length));
    } else if (length <= 12480) {
        length -= 193;
        data.push_back(static_cast<unsigned char>(length >> 8));
        data.push_back(static_cast<unsigned char>(length & 0xff));
    } else if (length <= 918744) {
        length -= 12481;
        data.push_back(static_cast<unsigned char>(length >> 16));
        data.push_back(static_cast<unsigned char>((length >> 8) & 0xff));
        data.push_back(static_cast<unsigned char>(length & 0xff));
    }
}

/// Encodes a variable length bytes.
inline void encodeBytes(std::vector<uint8_t> bytes, std::vector<uint8_t>& data) {
    encodeVariableLength(bytes.size(), data);
    data.insert(data.end(), bytes.begin(), bytes.end());
}

inline void encodeZeros(std::size_t len, std::vector<uint8_t>& data) {
    append(data, Data(len));
}

} // namespace TW::Ripple
