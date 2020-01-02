// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "../PublicKey.h"
#include "../Data.h"

namespace TW::Bravo {

class Address {
public:
    /// Determines whether a string makes a valid Bravo address.
    static bool isValid(const std::string& string, const std::vector<std::string>& validPrefixes = prefixes);

    /// Initializes a Bravo address from a string representation.
    Address(const std::string& string, const std::vector<std::string>& validPrefixes = prefixes);

    /// Initializes a Bravo address from a public key.
    Address(const PublicKey& publicKey, std::string prefix = prefixes[0]);

    /// Initializes a Bravo address from a byte array.
    Address(const Data& data, std::string prefix = prefixes[0]);

    /// Returns a string representation of the Bravo address.
    std::string string() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    static const std::vector<std::string> prefixes;

private:    
    // Class constants
    static const size_t Size = 33;

    Data bytes;
    std::string prefix;

    static bool extractKeyData(const std::string& string, const std::vector<std::string>& validPrefixes, Address *address = nullptr);
};


inline bool operator==(const Address& lhs, const Address& rhs) {
    return  lhs.bytes == rhs.bytes && lhs.prefix == rhs.prefix;
}

} // namespace


/// Wrapper for C interface.
struct TWBravoAddress {
    TW::Bravo::Address impl;
};