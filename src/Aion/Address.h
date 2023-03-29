// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Aion {

class Address {
  public:
    /// Number of bytes in Aion address.
    static const size_t size = 32;

    /// Address data
    std::array<uint8_t, size> bytes;

    /// Determines whether a collection of bytes makes a valid Aion address.
    static bool isValid(const std::vector<uint8_t>& data) {
        return data.size() == size && data[0] == 0xa0;
    }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const std::vector<uint8_t>& data);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

} // namespace TW::Aion
