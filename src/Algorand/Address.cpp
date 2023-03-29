// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Base32.h"

#include <array>

namespace TW::Algorand {

bool Address::isValid(const std::string& string) {
    if (string.size() != encodedSize) {
        return false;
    }
    Data decoded;
    if (!Base32::decode(string, decoded)) {
        return false;
    }
    // compute public key hash
    auto hash = Hash::sha512_256(decoded.data(), decoded.size() - checksumSize);
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
    Data decoded;
    if (!Base32::decode(string, decoded)) {
        throw std::invalid_argument("Invalid address string");
    }
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
    Data data;
    data.resize(dataSize);

    // base32_encode(publickey + checksum)
    std::copy(bytes.begin(), bytes.end(), data.data());
    std::copy(hash.end() - checksumSize, hash.end(), data.data() + PublicKey::ed25519Size);
    std::string encoded = Base32::encode(data);
    return encoded;
}

} // namespace TW::Algorand
