// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>

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

Address::Address(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == Address::size);

    memcpy(bytes, buffer, Address::size);
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(isValid(data));
    std::copy(data.begin(), data.end(), bytes);
}

Address::Address(const PublicKey& publicKey, uint8_t prefix) {
    bytes[0] = prefix;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes + 1);
}

std::string Address::string() const {
    size_t size = 0;
    b58enc(nullptr, &size, bytes, Address::size);
    size += 16;

    std::string str(size, ' ');
    base58_encode_check(bytes, Address::size, HASHER_SHA2D, &str[0], size);

    return str;
}
