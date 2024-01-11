// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../PublicKey.h"
#include "../Base58Address.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Nebulas {

class Address :public Base58Address<26> {
  public:
    /// Number of bytes in an address.
    static const uint8_t AddressPrefix = 25;
    static const uint8_t NormalType = 87;
    static const uint8_t ContractType = 88;

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data& data);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Nebulas
