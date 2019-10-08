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
#include <iostream>

using namespace TW;
using namespace TW::Filecoin;

bool Address::isValid(const std::string& string) {
    if (string.size() != encodedSize) {
        return false;
    }
    const size_t dataSize = payloadSize + checksumSize;
    std::array<byte, dataSize> decoded;
    // base32 decode
    if (base32_decode(string.data()+2, string.size()-2, decoded.data(), dataSize, BASE32_ALPHABET_RFC4648) == nullptr) {
        return false;
    }
    std::array<byte, payloadSize+1> check;
    std::copy(decoded.data(), decoded.data()+payloadSize, check.data()+1);
    check[0] = 1;
    // compute checksum
    auto hash = TW::Hash::blake2b(check.data(), check.data()+payloadSize+1, checksumSize);
    // compare checksum
    if (!std::equal(decoded.end() - checksumSize, decoded.end(), hash.begin())) {
        return false;
    }
    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    const size_t dataSize = payloadSize + checksumSize;
    std::array<byte, dataSize> decoded;
    base32_decode(string.data()+2, string.size()-2, decoded.data(), dataSize, BASE32_ALPHABET_RFC4648);
    std::copy(decoded.begin(), decoded.begin() + payloadSize, bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Invalid public key type");
    }
    auto hash = TW::Hash::blake2b(publicKey.bytes.data(), publicKey.bytes.data()+PublicKey::secp256k1Size, payloadSize);
    std::copy(hash.begin(), hash.end(), bytes.data());
}

std::string Address::string() const {
    std::array<byte, encodedSize> encoded;
    encoded[0] = 'f';
    encoded[1] = '1';
    std::array<byte, payloadSize+1> check;
    check[0] = 1;

    std::copy(bytes.data(), bytes.data()+payloadSize, check.data()+1);
    base32_encode(bytes.data(), payloadSize, reinterpret_cast<char*>(encoded.data()+2), encodedSize-2-7+1, BASE32_ALPHABET_RFC4648_LOWERCASE);
    // compute checksum
    auto hash = TW::Hash::blake2b(check.data(), check.data()+payloadSize+1, checksumSize);
    base32_encode(hash.data(), checksumSize, reinterpret_cast<char*>(encoded.data()+2+32), 7+1, BASE32_ALPHABET_RFC4648_LOWERCASE);

    return std::string(encoded.begin(), encoded.begin() + encodedSize);
}
