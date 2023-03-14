// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SS58Address.h"

using namespace TW;
using namespace std;

bool SS58Address::isValid(const std::string& string, uint32_t network) {
    const auto decoded = Base58::decode(string);
    byte decodedNetworkSize = 0;
    uint32_t decodedNetwork = 0;
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
Data SS58Address::computeChecksum(const T& data) {
    auto prefix = Data(gSS58Prefix.begin(), gSS58Prefix.end());
    append(prefix, Data(data.begin(), data.end()));
    auto hash = Hash::blake2b(prefix, 64);
    auto checksum = Data(checksumSize);
    std::copy(hash.begin(), hash.begin() + checksumSize, checksum.data());
    return checksum;
}

/// Initializes an address with a string representation.
SS58Address::SS58Address(const std::string& string, uint32_t network) {
    if (!isValid(string, network)) {
        throw std::invalid_argument("Invalid address string");
    }
    const auto decoded = Base58::decode(string);
    bytes.resize(decoded.size() - checksumSize);
    std::copy(decoded.begin(), decoded.end() - checksumSize, bytes.begin());
}

/// Initializes an address with a public key and network.
SS58Address::SS58Address(const PublicKey& publicKey, uint32_t network) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("SS58Address expects an ed25519 public key.");
    }
    if (!encodeNetwork(network, bytes)) {
        throw std::invalid_argument(std::string("network out of range ") + std::to_string(network));
    }
    TW::append(bytes, publicKey.bytes);
}

/// Returns a string representation of the address.
std::string SS58Address::string() const {
    auto result = Data(bytes.begin(), bytes.end());
    auto checksum = computeChecksum(bytes);
    append(result, checksum);
    return Base58::encode(result);
}

/// Returns public key bytes
Data SS58Address::keyBytes() const {
    byte networkSize;
    uint32_t networkTemp;
    decodeNetwork(bytes, networkSize, networkTemp);
    return Data(bytes.begin() + networkSize, bytes.end());
}

// Return true and the network size (1 or 2) and network if input is valid
bool SS58Address::decodeNetwork(const Data& data, byte& networkSize, uint32_t& network) {
    networkSize = 0;
    network = 0;
    if (data.size() >= 1 && data[0] < networkSimpleLimit) { // 0 -- 63
        networkSize = 1;
        network = (uint32_t)(data[0]);
        return true;
    }
    // src https://github.com/paritytech/substrate/blob/master/primitives/core/src/crypto.rs
    if (data.size() >= 2 && data[0] >= networkSimpleLimit && data[0] < networkFullLimit) { // 64 -- 127
        networkSize = 2;
        byte lower = (byte)((data[0] & 0b00111111) << 2) | (byte)((data[1] & 0b11000000) >> 6);
        byte upper = data[1] & 0b00111111;
        network = ((uint32_t)upper << 8) + lower;
        return (network >= networkSimpleLimit);
    }
    return false;
}

bool SS58Address::encodeNetwork(uint32_t network, Data& data) {
    if (network < networkSimpleLimit) { // 0 -- 63
        // Simple account/address/network
        data = {(byte)network};
        return true;
    }
    if (network < 0x4000) { // 64 -- 16383
        // Full address/address/network identifier.
        byte first = networkSimpleLimit + (byte)((network & 0b0000000011111100) >> 2);
        byte second = (byte)((network & 0b0011111100000000) >> 8) | (byte)((byte)(network & 0b0000000000000011) << 6);
        data = {first, second};
        return true;
    }
    // not supported
    return false;
}
