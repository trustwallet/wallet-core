// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../HexCoding.h"
#include "../Hash.h"
#include "Address.h"

#include <TrezorCrypto/base32.h>

#include <array>

using namespace TW::Algorand;

bool Address::isValid(const std::string& string) {
    if (string.size() != encodedSize) {
        return false;
    }
    const size_t dataSize = PublicKey::ed25519Size + checksumSize;
    std::array<byte, dataSize> decoded;
    // base32 decode
    if (base32_decode(string.data(), string.size(), decoded.data(), dataSize, BASE32_ALPHABET_RFC4648) == nullptr) {
        return false;
    }
    // compute public key hash
    auto hash = Hash::sha512_256(decoded.begin(), decoded.end() - checksumSize);
    // last 4 bytes are checksum
    std::array<byte, checksumSize> checksum;
    std::copy(hash.end() - checksumSize, hash.end(), checksum.data());
    // compare checksum
    if (!std::equal(decoded.end() - checksumSize, decoded.end(), checksum.begin())) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    const size_t dataSize = PublicKey::ed25519Size + checksumSize;
    std::array<byte, dataSize> decoded;
    base32_decode(string.data(), string.size(), decoded.data(), dataSize, BASE32_ALPHABET_RFC4648);
    std::copy(decoded.begin(), decoded.begin() + PublicKey::ed25519Size, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    std::copy(publicKey.bytes.begin(), publicKey.bytes.end(), bytes.data());
}

std::string Address::string() const {
    auto hash = Hash::sha512_256(bytes);
    const size_t dataSize = PublicKey::ed25519Size + checksumSize;
    std::array<byte, dataSize> data;
    std::vector<char> encoded;
    encoded.resize(64);

    // base32_encode(publickey + checksum)
    std::copy(bytes.begin(), bytes.end(), data.data());
    std::copy(hash.end() - checksumSize, hash.end(), data.data() + PublicKey::ed25519Size);
    base32_encode(data.data(), dataSize, encoded.data(), encoded.size(), BASE32_ALPHABET_RFC4648);
    return std::string(encoded.begin(), encoded.begin() + encodedSize);
}
