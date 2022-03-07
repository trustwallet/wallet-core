// Copyright © 2017-2022 Trust Wallet.
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

const std::string SS58Prefix = "SS58PRE";

namespace TW {

/// Base-58-encodeed Substrate address.
class SS58Address {
  public:
    static const size_t checksumSize = 2;

    /// Address data consisting of one or more network byte(s) followed by the public key.
    Data bytes;

    /// Determines whether a string makes a valid address
    static bool isValid(const std::string& string, uint64_t network) {
        const auto decoded = Base58::bitcoin.decode(string);
        byte decodedNetworkSize = 0;
        uint64_t decodedNetwork = 0;
        if (!decodeNetwork(decoded, decodedNetworkSize, decodedNetwork)) {
            return false;
        }
        // check size
        if ((decodedNetworkSize + PublicKey::ed25519Size + checksumSize) != decoded.size()) {
            return false;
        }
        // compare network
        if (decodedNetwork != network) {
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
    SS58Address(const std::string& string, uint64_t network) {
        if (!isValid(string, network)) {
            throw std::invalid_argument("Invalid address string");
        }
        const auto decoded = Base58::bitcoin.decode(string);
        bytes.resize(decoded.size() - checksumSize);
        std::copy(decoded.begin(), decoded.end() - checksumSize, bytes.begin());
    }

    /// Initializes an address with a public key and network.
    SS58Address(const PublicKey& publicKey, int32_t network) {
        if (publicKey.type != TWPublicKeyTypeED25519) {
            throw std::invalid_argument("SS58Address expects an ed25519 public key.");
        }
        if (!encodeNetwork(network, bytes)) {
            throw std::invalid_argument(std::string("network out of range ") + std::to_string(network));
        }
        TW::append(bytes, publicKey.bytes);
    }

    /// Returns a string representation of the address.
    std::string string() const {
        auto result = Data(bytes.begin(), bytes.end());
        auto checksum = computeChecksum(bytes);
        append(result, checksum);
        return Base58::bitcoin.encode(result);
    }

    /// Returns public key bytes
    Data keyBytes() const {
        byte networkSize;
        uint64_t networkTemp;
        decodeNetwork(bytes, networkSize, networkTemp);
        return Data(bytes.begin() + networkSize, bytes.end());
    }

    // Return true and the network size (1 or 2) and network if input is valid
    static bool decodeNetwork(const Data& data, byte& networkSize, uint64_t& network) {
        networkSize = 0;
        network = 0;
        if (data.size() >= 1 && data[0] < 0x40) { // 0 -- 63
            networkSize = 1;
            network = data[0];
            return true;
        }
        if (data.size() >= 2 && data[0] >= 0x40 && data[0] < 0x80) { // 64 -- 127
            networkSize = 2;
            network = ((data[0] & 0b00111111) << 8) + data[1];
            return true;
        }
        return false;
    }

    static bool encodeNetwork(uint64_t network, Data& data) {
        if (network < 0x40) { // 0 -- 63
            // Simple account/address/network
            data = {(byte)network};
            return true;
        }
        if (network < 0x4000) { // 64 -- 16383
            // Full address/address/network identifier.
            byte byte0 = 0b01000000 | (byte)((network & 0b0011111100000000) >> 8);
            byte byte1 = (byte)(network & 0b0000000011111111);
            data = {byte0, byte1};
            return true;
        }
        // not supported
        return false;
    }
};

inline bool operator==(const SS58Address& lhs, const SS58Address& rhs) {
    return lhs.bytes == rhs.bytes;
}

} // namespace TW
