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
namespace Tezos {

class Address {
public:
    /// Number of bytes in an address.
    static const size_t size = 23;

    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes a Tezos address with a string representation.
    Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    Address(const std::vector<uint8_t>& data);

    /// Initializes a Tezos address with a public key.
    Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;

    /// Forge an address to hex bytes.
    std::string forge() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

}} // namespace

/// Wrapper for C interface.
struct TWTezosAddress {
    TW::Tezos::Address impl;
};
