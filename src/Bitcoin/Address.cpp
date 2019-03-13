// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Bitcoin;

bool Address::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    return true;
}

bool Address::isValid(const std::string& string, const std::vector<byte>& validPrefixes) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    if (std::find(validPrefixes.begin(), validPrefixes.end(), buffer[0]) == validPrefixes.end()) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == Address::size);

    std::copy(buffer, buffer + Address::size, bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey, uint8_t prefix) {
    bytes[0] = prefix;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
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
