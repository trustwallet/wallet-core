// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

extern std::string TESTS_ROOT;

namespace TW::Keystore::tests {

const auto gName = "name";
const auto gPasswordString = "password";
const auto gPassword = TW::data(std::string(gPasswordString));

inline std::string testDataPath(const char *subpath) {
    return TESTS_ROOT + "/common/Keystore/Data/" + subpath;
}

} // namespace TW::Keystore::tests
