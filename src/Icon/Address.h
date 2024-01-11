// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../PublicKey.h"

#include "AddressType.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Icon {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 20;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<uint8_t, size> bytes{};

    /// Address type.
    enum AddressType type;

    /// Determines whether a collection of bytes makes a valid  address.
    static bool isValid(const std::vector<uint8_t>& data) { return data.size() == size; }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a public key.
    Address(const PublicKey& publicKey, enum AddressType type);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const class Address& lhs, const class Address& rhs) {
    return lhs.bytes == rhs.bytes && lhs.type == rhs.type;
}

} // namespace TW::Icon
