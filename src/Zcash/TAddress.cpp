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
using namespace TW::secp256k1;

bool TAddress::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != TAddress::size) {
        return false;
    }

    return true;
}

TAddress::TAddress(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == TAddress::size);

    memcpy(bytes, buffer, TAddress::size);
}

TAddress::TAddress(const std::vector<uint8_t>& data) {
    assert(isValid(data));
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

    std::string str(size, ' ');
    base58_encode_check(bytes, TAddress::size, HASHER_SHA2D, &str[0], size);

    return str;
}
