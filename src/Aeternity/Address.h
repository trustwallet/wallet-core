// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <PublicKey.h>
#include <string>

namespace TW::Aeternity {

class Address {
  public:
    static const size_t size = 32;
    Data bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address from a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address from a public key.
    explicit Address(const PublicKey &publicKey);

    /// Returns a string representation of the Aeternity address.
    std::string string() const;

  private:

    static bool checkType(const std::string& type);
    static bool checkPayload(const std::string& payload);
};

} // namespace TW::Aeternity
