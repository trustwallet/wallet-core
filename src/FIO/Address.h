// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../PublicKey.h"
#include "../Result.h"

#include <string>
#include <optional>

namespace TW::FIO {

class Address {
public:
    /// Number of bytes in an address, public key size + checksum
    static const size_t size = PublicKey::secp256k1Size + 4;

    /// Address data consisting of checksum (4 bytes) followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid FIO address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid FIO address.
    static bool isValid(const Data& bytes);

    /// Initializes a FIO address from a string representation.
    Address(const std::string& string);

    /// Creates a FIO address from a SECP256k1 public key (compressed or extended).
    static Result<Address> fromPublicKey(const PublicKey& publicKey);

    /// Returns a string representation of the FIO address.
    std::string string() const;

    /// Returns the public key of this address
    PublicKey publicKey() const;

    friend bool operator==(const Address& lhs, const Address& rhs);

    static std::string prefix() { return "FIO"; }

private:
    explicit Address(const std::array<byte, size>& bytes) : bytes(bytes) {}

    static uint32_t createChecksum(const Data& bytes);

    static std::optional<Data> decodeKeyData(const std::string& string);
};


inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW::FIO
