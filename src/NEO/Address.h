// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::NEO {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 21;

    /// NEO address version is 23
    /// https://github.com/neo-project/neo/blob/427a3cd08f61a33e98856e4b4312b8147708105a/neo/protocol.json#L4
    static const byte version = 0x17;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid NEO address.
    static bool isValid(const std::string& string);

    /// Initializes a NEO address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a NEO address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& data);

    /// Initializes a NEO address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::NEO

/// Wrapper for C interface.
struct TWNEOAddress {
    TW::NEO::Address impl;
};
