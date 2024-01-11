// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Ripple {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 21;

    /// Address data consisting of a prefix byte followed by the public key hash
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Ripple address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Ripple address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Ripple
