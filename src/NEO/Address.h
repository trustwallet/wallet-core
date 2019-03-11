// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW {
namespace NEO {

class Address {
public:
    /// Number of bytes in an address.
    static const size_t size = 34;

    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<byte, size> bytes;

    /// Determines whether a collection of bytes makes a valid NEO address.
    template<typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid NEO address.
    static bool isValid(const std::string& string);

    /// Initializes a NEO address with a string representation.
    Address(const std::string& string);

    /// Initializes a NEO address with a collection of bytes.
    Address(const std::vector<uint8_t>& data);

    /// Initializes a NEO address with a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

}} // namespace

/// Wrapper for C interface.
struct TWNEOAddress {
    TW::NEO::Address impl;
};
