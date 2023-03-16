// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <array>
#include <string>

namespace TW::Groestlcoin {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 21;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid address, and the prefix is
    /// within the valid set.
    static bool isValid(const std::string& string, const std::vector<byte>& validPrefixes);

    /// Initializes a  address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a  address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& data);

    /// Initializes a  address with a public key and a prefix.
    Address(const PublicKey& publicKey, uint8_t prefix);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Groestlcoin

/// Wrapper for C interface.
struct TWGroestlcoinAddress {
    TW::Groestlcoin::Address impl;
};
