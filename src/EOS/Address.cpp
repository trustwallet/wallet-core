// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Base58.h"
#include "../BinaryCoding.h"

#include <TrezorCrypto/ripemd160.h>

#include <stdexcept>

namespace TW::EOS {

bool Address::isValid(const std::string& string) {
    return extractKeyData(string);
}

/// Determines whether the given byte vector is a valid keyBuffer
/// Verifies the buffer's size and it's checksum bytes
bool Address::isValid(const Data& bytes, EOS::Type type) {
    if (bytes.size() != KeyDataSize) {
        return false;
    }

    // last Address::ChecksumSize bytes are a checksum
    uint32_t checksum = decode32LE(bytes.data() + PublicKeyDataSize);
    if (createChecksum(bytes, type) != checksum) {
        return false;
    }
    return true;
}

/// Creates a checksum of PublicKeyDataSize bytes at the buffer
/// IMPORTANT: THERE ARE NO SIZE CHECKS. THE BUFFER IS ASSUMED
///             TO HAVE PublicKeyDataSize bytes.
uint32_t Address::createChecksum(const Data& bytes, Type type) {
    // create our own checksum and compare the two
    uint8_t hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ctx;
    ripemd160_Init(&ctx);

    // add the bytes to the hash input
    ripemd160_Update(&ctx, bytes.data(), PublicKeyDataSize);

    //  append the prefix to the hash input as well in case of modern types
    switch (type) {
    case Type::Legacy: // no extra input
        break;

    case Type::ModernK1:
        ripemd160_Update(&ctx,
                         (const uint8_t*)Modern::K1::prefix.c_str(),
                         static_cast<uint32_t>(Modern::K1::prefix.size()));
        break;

    case Type::ModernR1:
        ripemd160_Update(&ctx,
                         (const uint8_t*)Modern::R1::prefix.c_str(),
                         static_cast<uint32_t>(Modern::R1::prefix.size()));
        break;
    }

    // finalize the hash
    ripemd160_Final(&ctx, hash);

    return decode32LE(hash);
}

/// Extracts and verifies the key data from a base58 string.
/// If the second arg is provided, the keyData and isTestNet
/// properties of that object are set from the extracted data.
bool Address::extractKeyData(const std::string& string, Address* address) {
    // verify if the string has one of the valid prefixes
    Type type;
    size_t prefixSize;
    if (string.substr(0, Legacy::prefix.size()) == Legacy::prefix) {
        type = Type::Legacy;
        prefixSize = Legacy::prefix.size();
    } else if (string.substr(0, Modern::R1::fullPubPrefix.size()) == Modern::R1::fullPubPrefix) {
        type = Type::ModernR1;
        prefixSize = Modern::R1::fullPubPrefix.size();
    } else if (string.substr(0, Modern::K1::fullPubPrefix.size()) == Modern::K1::fullPubPrefix) {
        type = Type::ModernK1;
        prefixSize = Modern::K1::fullPubPrefix.size();
    } else {
        return false;
    }

    const Data& decodedBytes = Base58::decode(string.substr(prefixSize));
    if (decodedBytes.size() != KeyDataSize) {
        return false;
    }

    if (isValid(decodedBytes, type)) {
        if (address) {
            address->keyData = decodedBytes;
            address->type = type;
        }

        return true;
    }

    return false;
}

/// Initializes a EOS address from a string representation.
Address::Address(const std::string& string) {
    if (!Address::extractKeyData(string, this)) {
        throw std::invalid_argument("Invalid address string!");
    }
}

/// Initializes a EOS address from raw bytes
Address::Address(const Data& data, Type type)
    : keyData(data), type(type) {
    if (!isValid(data, type)) {
        throw std::invalid_argument("Invalid byte size!");
    }
}

/// Initializes a EOS address from a public key.
Address::Address(const PublicKey& publicKey, Type type)
    : type(type) {
    assert(PublicKeyDataSize == TW::PublicKey::secp256k1Size);

    // copy the raw, compressed key data
    keyData = publicKey.compressed().bytes;

    // append the checksum
    uint32_t checksum = createChecksum(keyData, type);
    Data checksumBytes;
    encode32LE(checksum, checksumBytes);

    append(keyData, checksumBytes);
}

/// Returns a string representation of the EOS address.
std::string Address::string() const {
    return prefix() + Base58::encode(keyData);
}

} // namespace TW::EOS
