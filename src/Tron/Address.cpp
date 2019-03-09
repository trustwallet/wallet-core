// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Hash.h"

#include <TrezorCrypto/base58.h>

#include <cassert>
#include <stdexcept>

using namespace TW::Tron;

bool Address::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    if (buffer[0] != prefix) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size || buffer[0] != prefix) {
        throw std::invalid_argument("Invalid address string");
    }

    std::copy(buffer, buffer + Address::size, bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type() != PublicKeyType::secp256k1Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    const auto pkdata = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    const auto keyhash = Hash::keccak256(pkdata);
    bytes[0] = prefix;
    std::copy(keyhash.end() - size + 1, keyhash.end(), bytes.begin() + 1);
}

std::string Address::string() const {
    size_t size = 0;
    b58enc(nullptr, &size, bytes.data(), Address::size);
    size += 16;

    std::string str(size, '\0');
    const auto actualSize = base58_encode_check(bytes.data(), Address::size, HASHER_SHA2D, &str[0], size);
    str.erase(str.begin() + actualSize - 1, str.end());

    return str;
}
