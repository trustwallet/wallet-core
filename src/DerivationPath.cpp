// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "DerivationPath.h"

#include <charconv>
#include <stdexcept>

using namespace TW;

DerivationPath::DerivationPath(const std::string& string) {
    const auto* it = string.data();
    const auto* end = string.data() + string.size();

    if (it != end && *it == 'm') {
        ++it;
    }
    if (it != end && *it == '/') {
        ++it;
    }

    while (it != end) {
        uint64_t value{0};
        auto [ptr, ec] = std::from_chars(it, end, value);
        if (ec == std::errc::invalid_argument) {
            throw std::invalid_argument("Invalid component");
        }
        if (ec == std::errc::result_out_of_range || value >= HardenedOffset) {
            throw std::invalid_argument("Derivation index out of range");
        }
        it = ptr;

        auto hardened = (it != end && *it == '\'');
        if (hardened) {
            ++it;
        }
        indices.emplace_back(static_cast<uint32_t>(value), hardened);

        if (it == end) {
            break;
        }
        if (*it != '/') {
            throw std::invalid_argument("Components should be separated by '/'");
        }
        ++it;
    }
}

std::string DerivationPath::string() const noexcept {
    std::string result = "m/";
    for (auto& index : indices) {
        result += index.string();
        result += "/";
    }
    if (result.back() == '/') {
        result.erase(result.end() - 1);
    }
    return result;
}
