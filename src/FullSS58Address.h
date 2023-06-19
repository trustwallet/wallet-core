// Copyright © 2017-2023 Trust.
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

const std::string FullSS58Prefix = "SS58PRE";

namespace TW {

class FullSS58Address {
  public:
    static const size_t expectPublicKeySize = 32;

    /// Number of bytes in an address.
    static const size_t simpleFormatSize = 33;
    static const size_t fullFormatSize = 34;

    /// see https://docs.substrate.io/v3/advanced/ss58/#checksum-types
    static const size_t checksumSize = 2;

    /// Address data consisting of a network byte followed by the public key.
    std::vector<byte> bytes;


    /// Determines whether a string makes a valid address
    static bool isValid(const std::string& string, int32_t network) {
        const auto decoded = Base58::decode(string);
        if (decoded.size() != (simpleFormatSize + checksumSize) && decoded.size() != (fullFormatSize + checksumSize)) {
            return false;
        }

        if (decoded.size() == (simpleFormatSize + checksumSize)) {
            // check network
            if (decoded[0] != network) {
                return false;
            }
        } else {
            int32_t ss58Decoded = ((decoded[0] & 0b00111111) << 2) | (decoded[1] >> 6) | ((decoded[1] & 0b00111111) << 8);
            if (ss58Decoded != network) {
                return false;
            }
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
        auto prefix = Data(FullSS58Prefix.begin(), FullSS58Prefix.end());
        append(prefix, Data(data.begin(), data.end()));
        auto hash = Hash::blake2b(prefix, 64);
        auto checksum = Data(checksumSize);
        std::copy(hash.begin(), hash.begin() + checksumSize, checksum.data());
        return checksum;
    }

    FullSS58Address() = default;

    /// Initializes an address with a string representation.
    FullSS58Address(const std::string& string, int32_t network) {
        if (!isValid(string, network)) {
            throw std::invalid_argument("Invalid address string");
        }
        const auto decoded = Base58::decode(string);
        bytes.resize(decoded.size() - checksumSize);
        std::copy(decoded.begin(), decoded.end() - checksumSize, bytes.begin());
    }

    /// Initializes an address with a public key and network.
    /// see https://docs.substrate.io/v3/advanced/ss58/#format-in-detail
    FullSS58Address(const PublicKey& publicKey, int32_t network) {
        if (publicKey.type != TWPublicKeyTypeED25519) {
            throw std::invalid_argument("SS58Address expects an ed25519 public key.");
        }
        if (network < 0 || network > 16383) {
            throw std::invalid_argument("network out of range");
        }
        if (network < 64) {
            // Simple account/address/network
            bytes.resize(simpleFormatSize);
            bytes[0] = byte(network);
            std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.begin() + 1);
        } else {
            // Full address/address/network identifier.
            byte byte0 = (network & 0b0000000011111100) >> 2 | 0b01000000;
            byte byte1 = byte(network >> 8 | (network & 0b0000000000000011) << 6);

            bytes.resize(fullFormatSize);
            bytes[0] = byte0;
            bytes[1] = byte1;
            std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.begin() + 2);
        }
    }

    /// Returns a string representation of the address.
    std::string string() const {
        auto result = Data(bytes.begin(), bytes.end());
        auto checksum = computeChecksum(bytes);
        append(result, checksum);
        return Base58::encode(result);
    }

    /// Returns public key bytes
    Data keyBytes() const {
        Data bz;
        if (bytes.size() == simpleFormatSize) {
            bz = Data(bytes.begin() + 1, bytes.end());
        } else if (bytes.size() == fullFormatSize) {
            bz =  Data(bytes.begin() + 2, bytes.end());
        } else {
            throw std::length_error("invalid address bytes length");
        }

        if (bz.size() != expectPublicKeySize) {
            throw std::length_error("invalid public key bytes length");
        }

        return bz;
    }
};

inline bool operator==(const FullSS58Address& lhs, const FullSS58Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW
