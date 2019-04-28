// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "Data.h"
#include "PublicKey.h"

namespace TW::IoTeX {

class Address {
  public:
    /// 20-byte public key hash
    Data keyHash;

    /// Determines whether a string makes a valid  address
    static bool isValid(const std::string& string);

    /// Determines whether a byte slice makes a valid  key hash
    static bool isValid(const Data& data) { return data.size() == 20; }

    /// Initializes an address with a string representation
    explicit Address(const std::string& string);

    /// Initializes an address with a public key
    explicit Address(const PublicKey& publicKey);

    /// Initializes an address with a key hash
    explicit Address(const Data& keyHash);

    /// Returns a string representation of the address
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.keyHash == rhs.keyHash;
}

} // namespace TW::IoTeX

/// Wrapper for C interface
struct TWIoTeXAddress {
    TW::IoTeX::Address impl;
};
