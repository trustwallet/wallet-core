// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"
#include "Prefixes.h"

#include <string>

namespace TW::EOS {

class Address {
public:

    /// Determines whether a string makes a valid EOS address.
    static bool isValid(const std::string& string);

    /// Determines whether the given byte array is a valid keyBuffer
    static bool isValid(const  Data& bytes, Type type = Type::Legacy);

    /// Initializes a EOS address from a string representation.
    Address(const std::string& string);

    /// Initializes a EOS address from a public key.
    Address(const PublicKey& publicKey, Type type = Type::Legacy);

    /// Initializes a EOS address from a byte array.
    Address(const Data& data, Type type = Type::Legacy);

    /// Returns a string representation of the EOS address.
    std::string string() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    inline std::string prefix() const { return pubPrefixForType(type); }

protected:
    // Class constants
    static const size_t PublicKeyDataSize = 33;
    static const size_t ChecksumSize = 4;
    static const size_t KeyDataSize = PublicKeyDataSize + ChecksumSize;

    Data keyData;
    Type type;

    static uint32_t createChecksum(const Data& bytes, Type type);
    static bool extractKeyData(const std::string& string, Address *address = nullptr);
};


inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.keyData == rhs.keyData
             && lhs.type == rhs.type;
}

} // namespace TW::EOS
