// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "rust/bindgen/WalletCoreRSBindgen.h"
#include "rust/Wrapper.h"

#include <algorithm>
#include <array>
#include <sstream>
#include <string>
#include <tuple>


namespace TW::internal {
/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
inline Data parse_hex(const std::string& input) {
    if (input.empty()) {
        return {};
    }

    Rust::CByteArrayResultWrapper res = Rust::decode_hex(input.c_str());
    return res.unwrap_or_default().data;
}
}

namespace TW {

inline bool is_hex_encoded(const std::string& s)
{
    bool with_0x = s.compare(0, 2, "0x") == 0
           && s.size() > 2
           && s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
    bool without_0x = s.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
    return with_0x || without_0x;
}

/// Converts a collection of bytes to a hexadecimal string representation.
template <typename T>
inline std::string hex(const T& collection, bool prefixed = false) {
    auto rust_functor = [prefixed](auto&& collection){
        auto res = Rust::encode_hex(collection.data(), collection.size(), prefixed);
        std::string encoded_str(res);
        Rust::free_string(res);
        return encoded_str;
    };
    if constexpr (std::is_same_v<T, Data>) {
        return rust_functor(collection);
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        return rust_functor(data(collection));
    }
    else {
        return rust_functor(data_from(collection));
    }
}

/// same as hex, with 0x prefix
template <typename T>
inline std::string hexEncoded(T&& collection) {
    return hex(std::forward<T>(collection), true);
}

/// Converts a `uint64_t` value to a hexadecimal string.
inline std::string hex(uint64_t value) {
    const uint8_t* begin = reinterpret_cast<const uint8_t*>(&value);
    const uint8_t* end = begin + sizeof(value);
    Data v(begin, end);
    std::reverse(v.begin(), v.end());
    return hex(v);
}

/// Parses a string of hexadecimal values.
///
/// \returns the array or parsed bytes or an empty array if the string is not
/// valid hexadecimal.
inline Data parse_hex(const std::string& string, bool padLeft = false) {
    if (string.size() % 2 != 0 && padLeft) {
        std::string temp = string;
        if (temp.compare(0, 2, "0x") == 0) {
            temp.erase(0, 2);
        }
        temp.insert(0, 1, '0');
        return internal::parse_hex(temp);
    }
    return internal::parse_hex(string);
}

inline const char* hex_char_to_bin(char c) {
    switch (toupper(c)) {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    default:
        return "";
    }
}

inline std::string hex_str_to_bin_str(const std::string& hex) {
    std::stringstream ss;
    for (auto&& c: hex) {
        ss << hex_char_to_bin(c);
    }
    return ss.str();
}

} // namespace TW
