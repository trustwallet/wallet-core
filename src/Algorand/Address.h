// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Algorand {

class Address {
  public:
    /// Base32 encoded address string length.
    static const size_t encodedSize = 58;

    /// Sha512/256 checksum size.
    static const size_t checksumSize = 4;
    /// Address data consisting of public key.

    std::array<byte, PublicKey::ed25519Size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a Algorand address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Algorand address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

} // namespace TW::Algorand
