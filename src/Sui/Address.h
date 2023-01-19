// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"

#include <string>

namespace TW::Sui {

class Address {
  public:
    static constexpr size_t size = 20;
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Determines whether a collection of bytes makes a valid address.
    static bool isValid(const Data& data) { return data.size() == size; }

    /// Initializes a Sui address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a Sui address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    [[nodiscard]] std::string string(bool withPrefix = true) const;
};

} // namespace TW::Sui
