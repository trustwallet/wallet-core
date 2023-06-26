// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>
#include <optional>

namespace TW::NEAR {

class Address {
public:
    /// Number of bytes in an address, public key size.
    static const size_t size = PublicKey::ed25519Size;
    /// Number of bytes of a checksum in a legacy address.
    static const size_t legacyChecksumSize = 4;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid NEAR address.
    static bool isValid(const std::string& string);

    /// Initializes a NEAR address from a string representation.
    Address(const std::string& string);

    /// Initializes a NEAR address from a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the NEAR address.
    std::string string() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    static std::optional<Data> decodeLegacyAddress(const std::string& string);
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::NEAR
