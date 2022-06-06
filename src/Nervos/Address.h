// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CommonFunc.h"
#include "../Data.h"
#include "../PublicKey.h"

#include <string>

using namespace TW;

namespace TW::Nervos {

class Address {
  public:
    std::string hrp;
    AddressType addressType;
    int codeHashIndex;
    Data codeHash;
    HashType hashType;
    Data args;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);
    static bool isValid(const std::string& string, byte p2pkh, byte p2sh, const char* hrp);

    /// Initializes a Nervos address with a string representation.
    explicit Address(const std::string& string);
    explicit Address(const std::string& string, byte p2pkh, byte p2sh, const char* hrp);

    /// Initializes a Nervos address with a public key.
    explicit Address(const PublicKey& publicKey);
    explicit Address(const PublicKey& publicKey, byte p2pkh, const char* hrp);

    /// Returns a string representation of the address.
    std::string string() const;
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
