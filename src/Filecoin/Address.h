// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Filecoin {

class Address {
  public:
    const char* BASE32_ALPHABET_RFC4648_LOWERCASE = "abcdefghijklmnopqrstuvwxyz23456789";
    /// 'f'/'t'+ protocol + base32 encoded address string length + checksum.
    static const size_t encodedSize = 41;
    static const size_t payloadSize = 20;

    /// blake2b 4 bytes checksum size.
    static const size_t checksumSize = 4;

    /// Address data consisting of hashed public key.
    std::array<byte, PublicKey::secp256k1Size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Filecoin address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Filecoin address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Filecoin

/// Wrapper for C interface.
struct TWFilecoinAddress {
    TW::Filecoin::Address impl;
};
