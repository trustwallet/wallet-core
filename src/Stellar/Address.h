// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Stellar {

class Address {
  private:
    // 56 character base-32 encoded string
    static const size_t size = 56;

    // Decodes to 35 bytes
    static const size_t rawSize = 35;

    // Raw key size is 32 bytes
    static const size_t keySize = 32;

  public:
    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, keySize> bytes;

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes a Stellar address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Stellar address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Stellar
