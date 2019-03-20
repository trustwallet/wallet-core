// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../PrivateKey.h"
#include "../PublicKey.h"

#include <string>

namespace TW {
namespace NULS {

class Address {
public:
    /// Number of bytes in an address.
    static const size_t addressSize = 24;

    /// Address data consisting of a prefix byte followed by the public key hash.
    std::array<byte, addressSize> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Process NULS private key length is 31 or 33
    static PrivateKey importHexPrivateKey(std::string hexPrivateKey);

    /// Initializes an address from a string representation.
    Address(const std::string& string);

    /// Initializes an address from a public key.
    Address(const PublicKey& publicKey);

    /// Initializes an address with a collection of bytes.
    Address(const std::vector<uint8_t>& data);

    /// Returns a string representation of the address.
    std::string string() const;

    /// Determines is a valid address.
    bool isValid() const;

    uint16_t chainID() const;

    uint8_t type() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs)
{
    return lhs.bytes==rhs.bytes;
}

}
} // namespace

/// Wrapper for C interface.
struct TWNULSAddress {
  TW::NULS::Address impl;
};