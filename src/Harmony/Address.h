// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <vector>

namespace TW::Harmony {

class Address {
  public:
    /// Number of bytes in an address.
    static constexpr size_t size = 20;

    std::array<uint8_t, size> bytes;

    /// Determines whether a collection of bytes makes a valid address.
    static bool isValid(const Data &data);

    /// Determines whether a string makes a valid address, nonempty data payload if valid
    static std::pair<bool, std::vector<uint8_t>> isValid(const std::string &string);

    /// Initializes an address with a string representation.
    explicit Address(const std::string &string);

    /// Initializes an address with a collection of bytes.
    explicit Address(const Data &data);

    /// Initializes an address with a public key.
    explicit Address(const PublicKey &publicKey);

    /// Returns a bech32 representation of the address.
    std::string string() const;

    /// Provide hex representation of address
    std::string hexDump() const;
};

inline bool operator==(const Address &lhs, const Address &rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::Harmony

/// Wrapper for C interface.
struct TWHarmonyAddress {
    TW::Harmony::Address impl;
};
