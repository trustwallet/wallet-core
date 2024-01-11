// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace TW {

static std::vector<std::string> ssplit(const std::string& input, char delimiter) {
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
