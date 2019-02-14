// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <string>

namespace TW {

/// Converts a range of bytes to a hexadecimal string representation.
template<typename Iter>
inline std::string hex(const Iter begin, const Iter end) {
    static const char hexmap[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    std::string result;
    result.reserve((end - begin) * 2);

    for (auto it = begin; it < end; ++it) {
        auto val = static_cast<uint8_t>(*it);
        result.push_back(hexmap[val >> 4]);
        result.push_back(hexmap[val & 0x0f]);
    }

    return result;
}

/// Converts a collection of bytes to a hexadecimal string representation.
template<typename T>
inline std::string hex(const T& collection) {
    return hex(std::begin(collection), std::end(collection));
}

/// Converts a `uint64_t` value to a hexadecimal string.
inline std::string hex(uint64_t value) {
    auto bytes = reinterpret_cast<const uint8_t*>(&value);
    return hex(
        std::reverse_iterator<const uint8_t*>(bytes + sizeof(value)),
        std::reverse_iterator<const uint8_t*>(bytes)
    );
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not valid hexadecimal.
Data parse_hex(const std::string& string);

} // namespace
