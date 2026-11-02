// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "../Base58.h"
#include "../BinaryCoding.h"

#include <TrezorCrypto/ripemd160.h>

#include <stdexcept>

using namespace TW;

namespace TW::FIO {

bool Address::isValid(const std::string& string) {
    return decodeKeyData(string).has_value();
}

/// Determines whether the given byte vector is a valid keyBuffer
bool Address::isValid(const Data& bytes) {
    if (bytes.size() != size) {
        return false;
    }

    // last Address::ChecksumSize bytes are a checksum
    uint32_t checksum = decode32LE(bytes.data() + PublicKey::secp256k1Size);
    if (createChecksum(bytes) != checksum) {
        return false;
    }
    return true;
}

/// Creates a checksum of PublicKeyDataSize bytes at the buffer
uint32_t Address::createChecksum(const Data& bytes) {
    // create checksum and compare
    uint8_t hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ctx;
    ripemd160_Init(&ctx);

    // add the bytes to the hash input
    ripemd160_Update(&ctx, bytes.data(), PublicKey::secp256k1Size);

    // finalize the hash
    ripemd160_Final(&ctx, hash);

    return decode32LE(hash);
}

/// Decode and verifies the key data from a base58 string.
std::optional<Data> Address::decodeKeyData(const std::string& string) {
    size_t prefixSize = prefix().size();
    if (string.substr(0, prefixSize) != prefix()) {
        return {};
    }

    const Data& decodedBytes = Base58::decode(string.substr(prefixSize));
    if (decodedBytes.size() != size) {
        return {};
    }

    if (!isValid(decodedBytes)) {
        return {};
    }

    return decodedBytes;
}

/// Initializes a FIO address from a string representation.
Address::Address(const std::string& string) {
    auto data = Address::decodeKeyData(string);
    if (!data.has_value()) {
        throw std::invalid_argument("Invalid address string!");
    }
    std::copy(std::begin(*data), std::end(*data), std::begin(bytes));
}

/// Creates a FIO address from a SECP256k1 public key (compressed or extended).
Result<Address> Address::fromPublicKey(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1 && publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        return Result<Address>::failure("Invalid public key type");
    }
    // copy the raw, compressed key data
    Data data = publicKey.compressed().bytes;
    if (data.size() != PublicKey::secp256k1Size) {
        return Result<Address>::failure("Invalid public key size");
    }

    // append the checksum
    uint32_t checksum = createChecksum(data);
    Data checksumBytes;
    encode32LE(checksum, checksumBytes);
    append(data, checksumBytes);

    std::array<byte, size> addressBytes;
    std::copy(std::begin(data), std::end(data), std::begin(addressBytes));
    return Result<Address>::success(Address(addressBytes));
}

/// Returns a string representation of the FIO address.
std::string Address::string() const {
    return prefix() + Base58::encode(bytes);
}

PublicKey Address::publicKey() const {
    assert(bytes.size() >= PublicKey::secp256k1Size);
    const Data keyData = TW::data(bytes.data(), PublicKey::secp256k1Size);
    return PublicKey(keyData, TWPublicKeyTypeSECP256k1);
}

} // namespace TW::FIO
