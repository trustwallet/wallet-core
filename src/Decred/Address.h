// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <array>
#include <string>

namespace TW::Decred {

class Address {
  public:
    /// Public key hash with prefixes.
    std::array<byte, 22> bytes;

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string) noexcept;

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Decred
