// Copyright © 2017-2020 Trust Wallet.
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
#include <iostream>

const std::string SS58Prefix = "SS58PRE";

namespace TW {

class SS58Address {
  public:
    /// Number of bytes in an address.
    static const size_t size = 33;

    static const size_t checksumSize = 2;

    /// Address data consisting of a network byte followed by the public key.
    std::array<byte, size> bytes;

    /// Determines whether a string makes a valid address
    static bool isValid(const std::string& string, byte network) {
        const auto decoded = Base58::bitcoin.decode(string);
        if (decoded.size() != SS58Address::size + checksumSize) {
            return false;
        }
        // check network
        if (decoded[0] != network) {
            return false;
        }
        auto checksum = computeChecksum(Data(decoded.begin(), decoded.end() - checksumSize));
        // compare checksum
        if (!std::equal(decoded.end() - checksumSize, decoded.end(), checksum.begin())) {
            return false;
        }
        return true;
    }

    template <typename T>
    static Data computeChecksum(const T& data) {
        auto prefix = Data(SS58Prefix.begin(), SS58Prefix.end());
        append(prefix, Data(data.begin(), data.end()));
        auto hash = Hash::blake2b(prefix, 64);
        auto checksum = Data(checksumSize);
        std::copy(hash.begin(), hash.begin() + checksumSize, checksum.data());
        return checksum;
    }

    SS58Address() = default;

    /// Initializes an address with a string representation.
    SS58Address(const std::string& string, byte network) {
        if (!isValid(string, network)) {
            throw std::invalid_argument("Invalid address string");
        }
        const auto decoded = Base58::bitcoin.decode(string);
        std::copy(decoded.begin(), decoded.end() - checksumSize, bytes.begin());
    }

    /// Initializes an address with a public key and network.
    SS58Address(const PublicKey& publicKey, byte network) {
        if (publicKey.type != TWPublicKeyTypeED25519) {
            throw std::invalid_argument("SS58Address expects an ed25519 public key.");
        }
        bytes[0] = network;
        std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.begin() + 1);
    }

    /// Returns a string representation of the address.
    std::string string() const {
        auto result = Data(bytes.begin(), bytes.end());
        auto checksum = computeChecksum(bytes);
        append(result, checksum);
        return Base58::bitcoin.encode(result);
    }
};

inline bool operator==(const SS58Address& lhs, const SS58Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW
