// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "DerivationPath.h"

#include <cstdio>
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
        uint32_t value;
        if (std::sscanf(it, "%ud", &value) != 1) {
            throw std::invalid_argument("Invalid component");
        }
        while (it != end && isdigit(*it)) {
            ++it;
        }

        auto hardened = (it != end && *it == '\'');
        if (hardened) {
            ++it;
        }
        indices.emplace_back(value, hardened);

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
