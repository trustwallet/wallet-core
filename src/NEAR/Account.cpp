// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Account.h"

#include <regex>

namespace TW::NEAR {

static auto pattern = std::regex(R"(^(([a-z\d]+[\-_])*[a-z\d]+\.)*([a-z\d]+[\-_])*[a-z\d]+$)");

bool Account::isValid(const std::string& string) {
    // https://docs.near.org/docs/concepts/account#account-id-rules
    if (string.size() < 2 || string.size() > 64) {
        return false;
    }
    std::smatch match;
    return regex_search(string, match, pattern);
}

} // namespace TW::NEAR
