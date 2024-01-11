// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>

namespace TW::NEAR {

class Account {
  public:
    /// Determines whether a string makes a valid NEAR account id.
    static bool isValid(const std::string& string);
};
} // namespace TW::NEAR
