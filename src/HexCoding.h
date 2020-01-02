// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <array>
#include <string>
#include <tuple>

namespace TW {

std::tuple<uint8_t, bool> value(uint8_t c);

/// Converts a range of bytes to a hexadecimal string representation.
template <typename Iter>
inline std::string hex(const Iter begin, const Iter end) {
    static constexpr std::array<char, 16> hexmap = {
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
template <typename T>
inline std::string hex(const T& collection) {
    return hex(std::begin(collection), std::end(collection));
}

/// Converts a `uint64_t` value to a hexadecimal string.
inline std::string hex(uint64_t value) {
    auto bytes = reinterpret_cast<const uint8_t*>(&value);
    return hex(std::reverse_iterator<const uint8_t*>(bytes + sizeof(value)),
               std::reverse_iterator<const uint8_t*>(bytes));
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
template <typename Iter>
inline Data parse_hex(const Iter begin, const Iter end) {
    auto it = begin;

    // Skip `0x`
    if (end - begin >= 2 && *begin == '0' && *(begin + 1) == 'x') {
        it += 2;
    }

    Data result;
    result.reserve(((end - begin) + 1) / 2);

    while (it != end) {
        auto high = value(*it);
        if (!std::get<1>(high)) {
            return {};
        }
        it += 1;

        if (it == end) {
            result.push_back(std::get<0>(high));
            break;
        }

        auto low = value(*it);
        if (!std::get<1>(low)) {
            return {};
        }
        it += 1;

        result.push_back(static_cast<uint8_t>((std::get<0>(high) << 4) | std::get<0>(low)));
    }

    return result;
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
inline Data parse_hex(const std::string& string) {
    return parse_hex(string.begin(), string.end());
}

} // namespace TW
