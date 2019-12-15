// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
//#include "../PublicKey.h"

#include <string>

namespace TW::Cardano {

/// Cardano address.  Type is 0.
class Address {
  public:
    /*
    /// Base32 encoded address string length.
    static const size_t encodedSize = 58;

    /// Sha512/256 checksum size.
    static const size_t checksumSize = 4;
    /// Address data consisting of public key.

    std::array<byte, PublicKey::ed25519Size> bytes;
    */

    /// root key
    Data root;

    /// Additional info, CBOR-encoded
    Data attrs;

    /// Type; 0: public key.
    TW::byte type;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    static std::string build();

    /// Initializes a Cardano address with a string representation.
    explicit Address(const std::string& string);

    /*
    /// Initializes a Cardano address with a public key.
    explicit Address(const PublicKey& publicKey);
    */

    /// Returns a string representation of the address.
    std::string string() const;
};

/*
inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}
*/

} // namespace TW::Cardano

/// Wrapper for C interface.
struct TWCardanoAddress {
    TW::Cardano::Address impl;
};
