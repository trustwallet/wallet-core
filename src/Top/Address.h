// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Top {
const std::string TOP_PREFIX = "T80000";
class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 20;
    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<uint8_t, size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);
    /// Determines whether a collection of bytes makes a valid  address.
    static bool isValid(const Data& data) { return data.size() == size; }

    /// Initializes a Top address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Top address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    // TODO: Complete equality operator
    return true;
}

} // namespace TW::Top

/// Wrapper for C interface.
struct TWTopAddress {
    TW::Top::Address impl;
};
