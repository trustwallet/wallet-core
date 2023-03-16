// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
