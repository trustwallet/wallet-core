// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace TW {

constexpr std::string_view kWitespaces = " \t\n\r\f\v";

// trim from end of string (right)
inline void trim_right(std::string& s, std::string_view t = kWitespaces)
{
    s.erase(s.find_last_not_of(t) + 1);
}

// trim from beginning of string (left)
inline void trim_left(std::string& s, std::string_view t = kWitespaces)
{
    s.erase(0, s.find_first_not_of(t));
}

// trim from both ends of string (right then left)
inline void trim(std::string& s, std::string_view t = kWitespaces)
{
    trim_left(s, t);
    trim_right(s, t);
}

// trim from both ends of string (right then left)
inline std::string trim_copy(std::string s, std::string_view t = kWitespaces) {
    trim(s, t);
    return s;
}

inline std::vector<std::string> ssplit(const std::string& input, char delimiter) {
    std::istringstream iss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        if (!token.empty()) {
            tokens.emplace_back(token);
        }
    }
    return tokens;
}

} // namespace TW
