// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TAddress.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Zcash;

bool TAddress::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    auto size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != TAddress::size) {
        return false;
    }

    return true;
}

bool TAddress::isValid(const std::string& string, const std::vector<byte>& validPrefixes) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != TAddress::size) {
        return false;
    }

    if (std::find(validPrefixes.begin(), validPrefixes.end(), buffer[1]) == validPrefixes.end()) {
        return false;
    }

    return true;
}

TAddress::TAddress(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    auto size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != TAddress::size) {
        throw std::invalid_argument("Invalid address data");
    }

    memcpy(bytes, buffer, TAddress::size);
}

TAddress::TAddress(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes);
}

TAddress::TAddress(const PublicKey& publicKey, uint8_t prefix) {
    bytes[0] = 0x1c;
    bytes[1] = prefix;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes + 2);
}

std::string TAddress::string() const {
    size_t size = 0;
    b58enc(nullptr, &size, bytes, TAddress::size);
    size += 16;

    std::string str(size, '\0');
    const auto actualSize = base58_encode_check(bytes, TAddress::size, HASHER_SHA2D, &str[0], size);
    str.erase(str.begin() + actualSize - 1, str.end());

    return str;
}
