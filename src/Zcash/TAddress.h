// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <array>
#include <cstdint>
#include <string>

namespace TW::Zcash {

class TAddress {
  public:
    /// Number of bytes in an address.
    static const size_t size = 22;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    uint8_t bytes[size];

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid  address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid address, and the prefix is
    /// within the valid set.
    static bool isValid(const std::string& string, const std::vector<byte>& validPrefixes);

    /// Initializes a  address with a string representation.
    explicit TAddress(const std::string& string);

    /// Initializes a  address with a collection of bytes.
    explicit TAddress(const std::vector<uint8_t>& data);

    /// Initializes a  address with a public key and a prefix.
    TAddress(const PublicKey& publicKey, uint8_t prefix);

    /// Returns a string representation of the address.
    std::string string() const;

  private:
    TAddress() = default;
};

} // namespace TW::Zcash

/// Wrapper for C interface.
struct TWZcashTAddress {
    TW::Zcash::TAddress impl;
};
