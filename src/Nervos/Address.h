// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Constants.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

using namespace TW;

namespace TW::Nervos {

enum AddressType {
    FullVersion = 0,  // full version identifies the hash_type
    HashIdx = 1,      // short version for locks with popular codehash, deprecated
    DataCodeHash = 2, // full version with hash type 'Data', deprecated
    TypeCodeHash = 4, // full version with hash type 'Type', deprecated
};

class Address {
public:
    const char* hrp;
    AddressType addressType;
    byte codeHashIndex;
    Data codeHash;
    HashType hashType;
    Data args;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string) noexcept;
    static bool isValid(const std::string& string, const char* hrp) noexcept;

    /// Initializes a Nervos address with a string representation.
    explicit Address(const std::string& string);
    explicit Address(const std::string& string, const char* hrp);

    /// Initializes a Nervos address with a public key.
    explicit Address(const PublicKey& publicKey);
    explicit Address(const PublicKey& publicKey, const char* hrp);

    /// Returns a string representation of the address.
    std::string string() const;

private:
    Address() = default;

    // Decodes address from string
    bool decode(const std::string& string, const char* hrp) noexcept;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return (lhs.codeHash == rhs.codeHash) && (lhs.hashType == rhs.hashType) &&
           (lhs.args == rhs.args);
}

} // namespace TW::Nervos

/// Wrapper for C interface.
struct TWNervosAddress {
    TW::Nervos::Address impl;
};
