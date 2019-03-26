// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <array>
#include <string>

namespace TW {
namespace Tron {

class Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 21;

    /// Address prefix.
    static const byte prefix = 0x41;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a  address with a string representation.
    explicit Address(const std::string& string);

    /// Initializes a  address with a collection of bytes.
    explicit Address(const Data& data);

    /// Initializes a  address with a public key and a prefix.
    explicit Address(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

static inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace Tron
} // namespace TW
