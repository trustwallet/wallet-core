// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Base58.h"
#include "Data.h"
#include "PublicKey.h"

#include <array>
#include <string>

namespace TW {

template <std::size_t S>
class Base58Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = S;

    /// Address data consisting of a prefix byte followed by the public key
    /// hash.
    std::array<byte, size> bytes;

    /// Determines whether a collection of bytes makes a valid  address.
    template <typename T>
    static bool isValid(const T& data) {
        return data.size() == size;
    }

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string) {
        const auto decoded = Base58::decodeCheck(string);
        if (decoded.size() != Base58Address::size) {
            return false;
        }
        return true;
    }

    /// Determines whether a string makes a valid address, and the prefix is
    /// within the valid set.
    static bool isValid(const std::string& string, const std::vector<Data>& validPrefixes) {
        const auto decoded = Base58::decodeCheck(string);
        if (decoded.size() != Base58Address::size) {
            return false;
        }
        for (const auto& prefix : validPrefixes) {
            if (has_prefix(decoded, prefix)) {
                return true;
            }
        }
        return false;
    }

    Base58Address() = default;

    /// Initializes an address with a string representation.
    explicit Base58Address(const std::string& string) {
        const auto decoded = Base58::decodeCheck(string);
        if (decoded.size() != Base58Address::size) {
            throw std::invalid_argument("Invalid address string");
        }

        std::copy(decoded.begin(), decoded.end(), bytes.begin());
    }

    /// Initializes an address with a collection of bytes.
    explicit Base58Address(const Data& data) {
        if (!isValid(data)) {
            throw std::invalid_argument("Invalid address key data");
        }
        std::copy(data.begin(), data.end(), bytes.begin());
    }

    /// Initializes an address with a public key and a prefix.
    Base58Address(const PublicKey& publicKey, const Data& prefix) {
        if (publicKey.type != TWPublicKeyTypeSECP256k1) {
            throw std::invalid_argument("Bitcoin::Address needs a compressed SECP256k1 public key.");
        }
        const auto data = publicKey.hash(prefix, Hash::HasherSha256ripemd);
        std::copy(data.begin(), data.end(), bytes.begin());
    }

    /// Returns a string representation of the address.
    std::string string() const {
        return Base58::encodeCheck(bytes);
    }
};

template <std::size_t S>
inline bool operator==(const Base58Address<S>& lhs, const Base58Address<S>& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW
