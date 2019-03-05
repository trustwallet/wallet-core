// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../HexCoding.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ripple/base58.h>
#include <TrezorCrypto/ecdsa.h>

#include <cassert>

using namespace TW::Ripple;

bool Address::isValid(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = xrp_base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    if (size != Address::size) {
        return false;
    }

    return true;
}

Address::Address(const std::string& string) {
    size_t capacity = 128;
    uint8_t buffer[capacity];
    int size = xrp_base58_decode_check(string.data(), HASHER_SHA2D, buffer, (int)capacity);
    assert(size == Address::size);
    std::vector<uint8_t> vec(&buffer[0], &buffer[128]);
    auto str = TW::hex(vec);
    std::copy(buffer, buffer + Address::size, bytes.begin());
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(isValid(data));
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    /// see type prefix: https://developers.ripple.com/base58-encodings.html
    bytes[0] = 0x00;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.data() + 1);
}

std::string Address::string() const {
    size_t size = 0;
    base58_encode(nullptr, &size, bytes.data(), Address::size, xrp_b58digits);
    size += 16;

    std::string str(size, ' ');
    xrp_base58_encode_check(bytes.data(), Address::size, HASHER_SHA2D, &str[0], size);

    return std::string(str.c_str());
}
