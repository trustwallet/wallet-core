// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <string>

namespace TW::Vite {

class Address {
  public:
    /// Address data from blake2b hash of public key w/ checksum suffix.
    std::array<byte, 25> bytes;

    /// Determines whether a string makes a valid Vite address.
    static bool isValid(const std::string& address);

    /// Determines whether a string makes a valid Vite address.
    static bool isValid(const std::string& address, bool isContract);

    /// Initializes a Vite address with a string representation.
    explicit Address(const std::string& address);

    /// Initializes a Vite address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Vite
