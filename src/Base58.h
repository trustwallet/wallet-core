// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "Hash.h"

#include <array>
#include <string>

namespace TW {

class Base58 {
  public:
    /// Base58 coder with Bitcoin character map.
    static Base58 bitcoin;

    /// Base58 coder with Ripple character map.
    static Base58 ripple;

  public:
    /// Ordered list of valid characters.
    const std::array<char, 58> digits;

    /// Maps characters to base58 values.
    const std::array<signed char, 128> characterMap;

    /// Initializes a Base58 class with custom digit mapping.
    Base58(const std::array<char, 58>& digits, const std::array<signed char, 128>& characterMap)
        : digits(digits), characterMap(characterMap) {}

    /// Decodes a base 58 string verifying the checksum, returns empty on failure.
    Data decodeCheck(const std::string& string, Hash::Hasher hasher = Hash::HasherSha256d) const {
        return decodeCheck(string.data(), string.data() + string.size(), hasher);
    }

    /// Decodes a base 58 string verifying the checksum, returns empty on failure.
    Data decodeCheck(const char* begin, const char* end, Hash::Hasher hasher = Hash::HasherSha256d) const;

    /// Decodes a base 58 string into `result`, returns `false` on failure.
    Data decode(const std::string& string) const {
        return decode(string.data(), string.data() + string.size());
    }

    /// Decodes a base 58 string into `result`, returns `false` on failure.
    Data decode(const char* begin, const char* end) const;

    /// Encodes data as a base 58 string with a checksum.
    template <typename T>
    std::string encodeCheck(const T& data, Hash::Hasher hasher = Hash::HasherSha256d) const {
        return encodeCheck(data.data(), data.data() + data.size(), hasher);
    }

    /// Encodes data as a base 58 string with a checksum.
    std::string encodeCheck(const byte* pbegin, const byte* pend, Hash::Hasher hasher = Hash::HasherSha256d) const;

    /// Encodes data as a base 58 string.
    template <typename T>
    std::string encode(const T& data) const {
        return encode(data.data(), data.data() + data.size());
    }

    /// Encodes data as a base 58 string.
    std::string encode(const byte* pbegin, const byte* pend) const;
};

} // namespace TW
