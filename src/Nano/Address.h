// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <string>

namespace TW::Nano {

class Address {
  public:
    /// Address data consisting of a prefix byte followed by the public key
    /// bytes.
    std::array<byte, 32> bytes;

    /// Determines whether a string makes a valid Nano address.
    static bool isValid(const std::string& address);

    /// Initializes a Nano address with a string representation.
    explicit Address(const std::string& address);

    /// Initializes a Nano address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Nano
