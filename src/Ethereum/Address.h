// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Ethereum {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 20;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<uint8_t, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    static bool isValid(const Data& data) { return data.size() == size; }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
  protected:
    Address() = default;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Ethereum
