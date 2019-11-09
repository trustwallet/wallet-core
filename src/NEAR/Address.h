// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>
#include <optional>

namespace TW::NEAR {

class Address {
public:
    /// Number of bytes in an address, public key size + checksum
    static const size_t size = PublicKey::ed25519Size + 4;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid NEAR address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid NEAR address.
    static bool isValid(const Data& bytes);

    /// Initializes a NEAR address from a string representation.
    Address(const std::string& string);

    /// Initializes a NEAR address from a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the NEAR address.
    std::string string() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    static std::string prefix() { return "NEAR"; }

private:
    static uint32_t createChecksum(const Data& bytes);

    static std::optional<Data> decodeKeyData(const std::string& string);
};


inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::NEAR

/// Wrapper for C interface.
struct TWNEARAddress {
    TW::NEAR::Address impl;
};
