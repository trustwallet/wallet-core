// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Nebulas {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 26;
    static const u_int8_t AddressPrefix = 25;
    static const u_int8_t NormalType = 87;
    static const u_int8_t ContractType = 88;

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
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Nebulas

/// Wrapper for C interface.
struct TWNebulasAddress {
    TW::Nebulas::Address impl;
};
